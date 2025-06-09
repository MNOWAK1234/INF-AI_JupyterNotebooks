import com.espertech.esper.runtime.client.EPEventService;

import java.io.ByteArrayOutputStream;
import java.io.File;
import java.io.FileNotFoundException;
import java.io.IOException;
import java.io.RandomAccessFile;
import java.io.UnsupportedEncodingException;
import java.net.URI;
import java.net.URISyntaxException;
import java.net.URL;
import java.nio.ByteBuffer;
import java.nio.channels.FileChannel;
import java.text.DateFormat;
import java.text.ParseException;
import java.text.SimpleDateFormat;
import java.util.Calendar;
import java.util.Date;

public class CreateInputStream {

    private static final String DATA_ROZPOCZECIA = "2001-09-05";
    private static final String DATA_ZAKONCZENIA = "2001-09-20";

    private static final int LICZBA_PLIKOW = 12;
    private static final int MAX_LICZBA_BLEDOW = 5;
    private static final InformacjeOPliku[] tablicaInformacjiOPlikach = new InformacjeOPliku[LICZBA_PLIKOW];
    private static DateFormat df = null;

    public void generuj(EPEventService eventService) throws IOException {
        tablicaInformacjiOPlikach[0] = new InformacjeOPliku(
                "tableAPPLE_NASDAQ.csv", "Apple", "NASDAQ");
        tablicaInformacjiOPlikach[1] = new InformacjeOPliku(
                "tableCOCACOLA_NYSE.csv", "CocaCola", "NYSE");
        tablicaInformacjiOPlikach[2] = new InformacjeOPliku(
                "tableDISNEY_NYSE.csv", "Disney", "NYSE");
        tablicaInformacjiOPlikach[3] = new InformacjeOPliku(
                "tableFORD_NYSE.csv", "Ford", "NYSE");
        tablicaInformacjiOPlikach[4] = new InformacjeOPliku(
                "tableGOOGLE_NASDAQ.csv", "Google", "NASDAQ");
        tablicaInformacjiOPlikach[5] = new InformacjeOPliku(
                "tableHONDA_NYSE.csv", "Honda", "NYSE");
        tablicaInformacjiOPlikach[6] = new InformacjeOPliku(
                "tableIBM_NASDAQ.csv", "IBM", "NASDAQ");
        tablicaInformacjiOPlikach[7] = new InformacjeOPliku(
                "tableINTEL_NASDAQ.csv", "Intel", "NASDAQ");
        tablicaInformacjiOPlikach[8] = new InformacjeOPliku(
                "tableMICROSOFT_NASDAQ.csv", "Microsoft", "NASDAQ");
        tablicaInformacjiOPlikach[9] = new InformacjeOPliku(
                "tableORACLE_NASDAQ.csv", "Oracle", "NASDAQ");
        tablicaInformacjiOPlikach[10] = new InformacjeOPliku(
                "tablePEPSICO_NYSE.csv", "PepsiCo", "NYSE");
        tablicaInformacjiOPlikach[11] = new InformacjeOPliku(
                "tableYAHOO_NASDAQ.csv", "Yahoo", "NASDAQ");

        ReverseLineReader[] readers = new ReverseLineReader[LICZBA_PLIKOW];

        try {
            for (int i = 0; i < LICZBA_PLIKOW; i++) {
                File file = getFile(i);
                readers[i] = new ReverseLineReader(file, "UTF-8");
            }
        } catch (FileNotFoundException e) {
            System.err.println("Nie odnaleziono pliku");
            System.exit(1);
        }

        Date dataRozpoczecia = null;
        Date dataZakonczenia = null;


        try {
            df = new SimpleDateFormat("yyyy-MM-dd");
            dataRozpoczecia = df.parse(DATA_ROZPOCZECIA);
            dataZakonczenia = df.parse(DATA_ZAKONCZENIA);
        } catch (ParseException e) {
            System.err.println("Nie udało się wczytać podanych dat rozpoczęcia i zakończenia!");
            System.exit(1);
        }

        String[] linie = new String[LICZBA_PLIKOW];

        // Przesuniecie do pierwszych notowań z zakresu dat

        String[] splitResult;
        for (int i = 0; i < LICZBA_PLIKOW; i++) {
            while ((linie[i] = readers[i].readLine()) != null) {
                splitResult = linie[i].split(",");
                Date dataNotowania;
                try {
                    dataNotowania = df.parse(splitResult[0]);
                    if (dataNotowania.compareTo(dataRozpoczecia) >= 0) {
                        break;
                    }
                } catch (Exception ignored) {
                }
            }
        }

        Date iteratorDaty = dataRozpoczecia;

        // Główna pętla

        int liczbaBledow = 0;
        while ((iteratorDaty.compareTo(dataZakonczenia) <= 0)
                && (liczbaBledow < MAX_LICZBA_BLEDOW)) {

            for (int i = 0; i < LICZBA_PLIKOW; i++) {
                try {
                    Date dataNotowania = null;

                    if (linie[i] != null) {
                        dataNotowania = wyodrebnijDate(linie[i]);
                    }

                    if (dataNotowania == null) {
                        continue;
                    }

                    if ((dataNotowania.compareTo(iteratorDaty) < 0)) {
                        // Jeśli data ostatnio wczytanego notowania jest wcześniejsza niż
                        // bieżąca data, pobierz kolejne notowanie!
                        if ((linie[i] = readers[i].readLine()) != null) {
                            dataNotowania = wyodrebnijDate(linie[i]);
                        }
                    } else if ((dataNotowania.compareTo(iteratorDaty) > 0)) {
                        // Jeśli Data ostatnio wczytanego notowania jest późniejsza niż
                        // bieżąca data, wówczas czekaj!
                        continue;
                    }

                    if ((dataNotowania != null) && (dataNotowania.equals(iteratorDaty))) {
                        // Tworzenie obiektu notowania
                        if (linie[i] != null) {
                            splitResult = linie[i].split(",");
                            KursAkcji kurs = new KursAkcji(
                                    tablicaInformacjiOPlikach[i].getNazwaSpolki(),
                                    tablicaInformacjiOPlikach[i].getNazwaMarketu(),
                                    dataNotowania,
                                    Float.valueOf(splitResult[1].trim()),
                                    Float.valueOf(splitResult[2].trim()),
                                    Float.valueOf(splitResult[3].trim()),
                                    Float.valueOf(splitResult[4].trim()),
                                    Float.valueOf(splitResult[5].trim()));
                            eventService.sendEventBean(kurs, kurs.getClass().getName());
                        }
                    }
                } catch (Exception e) {
                    liczbaBledow++;
                    System.err.println("Błąd parsowania! [" + linie[i] + "]. Po raz: " + liczbaBledow);

                    if (liczbaBledow >= MAX_LICZBA_BLEDOW) {
                        System.err.println("Za dużo błędów!");
                        break;
                    }
                }
            }

            // Inkrementacja daty
            iteratorDaty = inkrementujDate(iteratorDaty);
        }
    }

    private static File getFile(int i) throws FileNotFoundException {
        ClassLoader classLoader = Main.class.getClassLoader();
        URL resourceURL = classLoader.getResource(tablicaInformacjiOPlikach[i].getNazwaPliku());

        if (resourceURL == null) {
            throw new FileNotFoundException("Plik nie został znaleziony!");
        }

        // Konwertuj URL na URI, a następnie na File
        URI resourceURI;
        try {
            resourceURI = resourceURL.toURI();
        } catch (URISyntaxException e) {
            throw new RuntimeException(e);
        }
        return new File(resourceURI);
    }

    // Metody pomocnicze
    private static Date inkrementujDate(Date data) {
        Calendar c = Calendar.getInstance();
        c.setTime(data);
        c.add(Calendar.DATE, 1);
        return c.getTime();
    }

    private static Date wyodrebnijDate(String linia) {
        String[] splitResult = linia.split(",");
        if (!splitResult[0].equals("Date")) {
            try {
                return df.parse(splitResult[0]);
            } catch (ParseException ignored) {
            }
        }
        return null;
    }

    // Klasa pomocnicza
    private static class InformacjeOPliku {
        private final String nazwaPliku;
        private final String nazwaSpolki;
        private final String nazwaMarketu;

        public InformacjeOPliku(String nazwaPliku, String nazwaSpolki, String nazwaMarketu) {
            this.nazwaPliku = nazwaPliku;
            this.nazwaSpolki = nazwaSpolki;
            this.nazwaMarketu = nazwaMarketu;
        }

        public String getNazwaPliku() {
            return nazwaPliku;
        }

        public String getNazwaSpolki() {
            return nazwaSpolki;
        }

        public String getNazwaMarketu() {
            return nazwaMarketu;
        }
    }

    // Klasa pomocnicza
    // AUTOR: WhiteFang34
    // ŹRÓDŁO: http://stackoverflow.com/questions/6011345/read-a-file-line-by-line-in-reverse-order
    private static class ReverseLineReader {
        private static final int BUFFER_SIZE = 8192;

        private final FileChannel channel;
        private final String encoding;
        private long filePos;
        private ByteBuffer buf;
        private int bufPos;
        private ByteArrayOutputStream baos = new ByteArrayOutputStream();

        public ReverseLineReader(File file, String encoding) throws IOException {
            RandomAccessFile raf = new RandomAccessFile(file, "r");
            channel = raf.getChannel();
            filePos = raf.length();
            this.encoding = encoding;
        }

        public String readLine() throws IOException {
            while (true) {
                if (bufPos < 0) {
                    if (filePos == 0) {
                        if (baos == null) {
                            return null;
                        }
                        String line = bufToString();
                        baos = null;
                        return line;
                    }

                    long start = Math.max(filePos - BUFFER_SIZE, 0);
                    long end = filePos;
                    long len = end - start;

                    buf = channel.map(FileChannel.MapMode.READ_ONLY, start, len);
                    bufPos = (int) len;
                    filePos = start;
                }

                while (bufPos-- > 0) {
                    byte c = buf.get(bufPos);
                    if (c == '\n') {
                        return bufToString();
                    } else if (c == '\r') {
                        bufPos--; // Skip the '\r'
                    }
                    baos.write(c);
                }
            }
        }

        private String bufToString() throws UnsupportedEncodingException {
            if (baos.size() == 0) {
                return "";
            }

            byte[] bytes = baos.toByteArray();
            for (int i = 0; i < bytes.length / 2; i++) {
                byte t = bytes[i];
                bytes[i] = bytes[bytes.length - i - 1];
                bytes[bytes.length - i - 1] = t;
            }

            baos.reset();

            return new String(bytes, encoding);
        }
    }

}
