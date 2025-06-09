package com.example.bigdata;

import org.apache.kafka.clients.consumer.ConsumerConfig;
import org.apache.kafka.common.serialization.Serde;
import org.apache.kafka.common.serialization.Serdes;
import org.apache.kafka.common.utils.Bytes;
import org.apache.kafka.streams.*;
import org.apache.kafka.streams.kstream.*;
import org.apache.kafka.streams.processor.ProcessorContext;
import org.apache.kafka.streams.state.KeyValueStore;

import java.io.FileReader;
import java.time.Duration;
import java.time.LocalDate;
import java.time.YearMonth;
import java.time.ZoneId;
import java.util.*;
import java.util.concurrent.CountDownLatch;

import com.opencsv.CSVReader;
import org.apache.kafka.streams.state.StoreBuilder;
import org.apache.kafka.streams.state.Stores;

public class BicycleSharing {

    public static void main(String[] args) throws Exception {
        // parametry
        if (args.length != 4) {
            System.out.println("Należy podać 4 parametry: bootstrap server, delay, D, P");
            System.exit(0);
        }
        String delay = args[1];
        int D = Integer.parseInt(args[2]);
        int P = Integer.parseInt(args[3]);
        if (!delay.equals("A") && !delay.equals("C")) {
            System.out.println("Parametr delay może przyjmować wartości A lub C");
            System.exit(0);
        }

        // wczytanie danych z pliku csv
        Map<String, String> IDtoName = new HashMap<>();
        Map<String, Integer> IDtoSize = new HashMap<>();

        try (CSVReader reader = new CSVReader(new FileReader("Divvy_Bicycle_Stations.csv"))) {
            String[] line;
            boolean isFirst = true;
            while ((line = reader.readNext()) != null) {
                if (isFirst) {
                    isFirst = false;
                    continue;
                }
                StationInfo stationInfo = StationInfo.parseFromCSVLine(line);
                IDtoName.putIfAbsent(stationInfo.getId(), stationInfo.getStationName());
                IDtoSize.putIfAbsent(stationInfo.getId(), Integer.valueOf(stationInfo.getDocksInService()));
            }
        }


        // config
        Properties config = new Properties();
        config.put(StreamsConfig.BOOTSTRAP_SERVERS_CONFIG, args[0]);
        config.put(StreamsConfig.APPLICATION_ID_CONFIG, "bicycle-sharing-application");
        config.put(StreamsConfig.DEFAULT_KEY_SERDE_CLASS_CONFIG, Serdes.String().getClass());
        config.put(StreamsConfig.DEFAULT_VALUE_SERDE_CLASS_CONFIG, Serdes.String().getClass());
        config.put(StreamsConfig.DEFAULT_TIMESTAMP_EXTRACTOR_CLASS_CONFIG, MyEventTimeExtractor.class);

        // chat debug
        config.put(ConsumerConfig.AUTO_OFFSET_RESET_CONFIG, "earliest");

        Serde<String> stringSerde = Serdes.String();
        Serde<BikeEvent> bikeEventSerde = new JsonSerde<>(BikeEvent.class);
        Serde<EventsAggregated> eventAggSerde = new JsonSerde<>(EventsAggregated.class);
        Serde<RentDifference> rentDifferenceSerde = new JsonSerde<>(RentDifference.class);

        // konstruktor topologii
        final StreamsBuilder builder = new StreamsBuilder();

        // procesor wejścia
        KStream<String, String> textLines = builder
                .stream("input-topic", Consumed.with(Serdes.String(), Serdes.String()));

        KStream<String, BikeEvent> bikeEventStream = textLines
                .filter((key, value) -> BikeEvent.lineIsCorrect(value))
                .mapValues(value -> {
                    return BikeEvent.parseFromLine(value);
                });

        // ---------- obraz czasu rzeczywistego ------------

        KGroupedStream<String, BikeEvent> groupedDailyByStationId = bikeEventStream
                .map((key, value) -> {
                    Calendar cal = Calendar.getInstance();
                    cal.setTime(value.getEventTime());
                    String yearMonthDayKey =
                            cal.get(Calendar.YEAR) + "-" +
                                    String.format("%02d", cal.get(Calendar.MONTH) + 1) + "-" +
                                    String.format("%02d", cal.get(Calendar.DAY_OF_MONTH)) + "-" +
                                    value.getStationId();
                    return KeyValue.pair(yearMonthDayKey, value);
                })
                .groupByKey(Grouped.with(stringSerde, bikeEventSerde));

        // agregacja
        KTable<String, EventsAggregated> stationState = groupedDailyByStationId
                .aggregate(
                        EventsAggregated::new,
                        (key, value, aggregate) -> {
                            EventsAggregated updatedAggregate = aggregate.add(value);
                            return updatedAggregate;
                        },
                        Materialized.<String, EventsAggregated, KeyValueStore<Bytes, byte[]>>as("dailyAggStore")
                                .withKeySerde(stringSerde)
                                .withValueSerde(eventAggSerde)
                );

        if (delay.equals("A")) {
            stationState
                    .toStream()
                    .mapValues(EventsAggregated::toString)
                    .to("output-topic", Produced.with(Serdes.String(), Serdes.String()));
        }
        if (delay.equals("C")) {
            StoreBuilder<KeyValueStore<String, String>> dayStoreBuilder =
                    Stores.keyValueStoreBuilder(
                            Stores.persistentKeyValueStore("lastDayStore"),
                            Serdes.String(),
                            Serdes.String()
                    );

            builder.addStateStore(dayStoreBuilder);

            stationState
                    .transformValues(() -> new ValueTransformerWithKey<String, EventsAggregated, EventsAggregated>() {
                        private KeyValueStore<String, String> lastDayStore;

                        @Override
                        public void init(ProcessorContext context) {
                            lastDayStore = (KeyValueStore<String, String>) context.getStateStore("lastDayStore");
                        }

                        @Override
                        public EventsAggregated transform(String key, EventsAggregated value) {
                            String[] parts = key.split("-", 4);
                            String eventDay = parts[0] + "-" + parts[1];
                            String event = parts[2];

                            String lastDayForEvent = lastDayStore.get(event);

                            if (lastDayForEvent == null || !lastDayForEvent.equals(eventDay)) {
                                lastDayStore.put(event, eventDay);
                                return value;
                            }
                            return null;
                        }

                        @Override
                        public void close() {}
                    },"lastDayStore")
                    .filter((key, value) -> value != null)
                    .toStream()
                    .mapValues(EventsAggregated::toString)
                    .to("output-topic", Produced.with(Serdes.String(), Serdes.String()));
        }

        // ---------- wykrywanie anomalii ------------

        KGroupedStream<String, BikeEvent> groupedById = bikeEventStream
                .map((key, value) -> KeyValue.pair(value.getStationId(), value))
                .groupByKey(Grouped.with(stringSerde, bikeEventSerde));

        // okno czasowe - każdego dnia rozpatrujemy okres D minut
        TimeWindows window = TimeWindows.ofSizeWithNoGrace(Duration.ofMinutes(D))
                .advanceBy(Duration.ofMinutes(10));

        // wykrywanie
        KStream<Windowed<String>, RentDifference> anomalies = groupedById
                .windowedBy(window)
                .aggregate(
                        RentDifference::new,
                        (key, value, aggregate) -> aggregate.add(value),
                        Materialized.with(Serdes.String(), rentDifferenceSerde)
                )
                .toStream()
                .filter((key, value) -> {
                    if (value == null) {
                        return false;
                    }
                    Integer size = IDtoSize.get(key.key());
                    if (size == null) {
                        return false;
                    }
                    if (size == 0) {
                        return false;
                    }
                    return Math.abs((double) value.getDifference() / (double) size) * 100 > P;
                });

        anomalies
                .map((windowedKey, value) -> {
                    String stationId = windowedKey.key();
                    long windowStart = windowedKey.window().start();
                    long windowEnd = windowedKey.window().end();
                    Integer size = IDtoSize.get(stationId);
                    double ratio = 0.0;

                    if (size != null && size != 0) {
                        ratio = Math.abs((double) value.getDifference() / (double) size) * 100;
                    }

                    String nieskompensowaneLabel;
                    long nieskompensowaneCount = Math.abs(value.getDifference());

                    if (value.getDifference() > 0) {
                        nieskompensowaneLabel = "wypożyczenia nieskompensowane zwrotami";
                    } else if (value.getDifference() < 0) {
                        nieskompensowaneLabel = "zwroty nieskompensowane wypożyczeniami";
                    } else {
                        nieskompensowaneLabel = "brak nieskompensowanych zdarzeń";
                        nieskompensowaneCount = 0;
                    }

                    String result = String.format(
                            "Okno: %tF %tT - %tF %tT, Stacja: %s, %s: %d, Wielkość stacji: %d, Stosunek: %.2f%%",
                            windowStart, windowStart, windowEnd, windowEnd,
                            stationId,
                            nieskompensowaneLabel,
                            nieskompensowaneCount,
                            size != null ? size : 0,
                            ratio
                    );

                    return KeyValue.pair(windowedKey, result);
                })
                .to("alert-topic", Produced.with(
                        WindowedSerdes.timeWindowedSerdeFrom(String.class, Duration.ofDays(D).toMillis()),
                        Serdes.String()
                ));

        final Topology topology = builder.build();
        System.out.println(topology.describe());

        KafkaStreams streams = new KafkaStreams(topology, config);

        final CountDownLatch latch = new CountDownLatch(1);

        Runtime.getRuntime().addShutdownHook(new Thread("streams-shutdown-hook") {
            @Override
            public void run() {
                streams.close();
                latch.countDown();
            }
        });

        try {
            streams.start();
            System.out.println("Kafka Streams started.");
            System.out.println("Kafka bootstrap: " + args[0]);
            latch.await();
        } catch (Throwable e) {
            System.exit(1);
        }
        System.exit(0);
    }
}

