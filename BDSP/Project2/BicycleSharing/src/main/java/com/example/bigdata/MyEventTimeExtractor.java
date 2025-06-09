package com.example.bigdata;

import org.apache.kafka.clients.consumer.ConsumerRecord;
import org.apache.kafka.streams.processor.TimestampExtractor;

public class MyEventTimeExtractor implements TimestampExtractor {

    public long extract(final ConsumerRecord<Object, Object> record,
                        final long previousTimestamp) {
        /*long timestamp = -1;
        String stringLine;

        if (record.value() instanceof String) {
            stringLine = (String) record.value();
            if (BikeEvent.lineIsCorrect(stringLine)) {
                timestamp = BikeEvent.parseFromLine(stringLine).
                        getTimestampInMillis();
            }
        }
        return timestamp;*/
        return System.currentTimeMillis();
    }
}