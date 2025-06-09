package com.example.bigdata;

import java.util.Random;

public enum Service {
    Netflix("Netflix"),
    Amazon("Amazon Prime"),
    Disney("Disney+"),
    HBO("HBO Max"),
    Spotify("Spotify"),
    YouTube("YouTube Premium"),
    Apple("Apple TV+"),
    Hulu("Hulu"),
    Paramount("Paramount+"),
    Peacock("Peacock");

    public final String name;
    private static Random generator = new Random();

    public static void setRandom(Random r){
        generator = r;
    }

    private Service(String name) {
        this.name = name;
    }

    public static String randomService() {
        Service[] services = values();
        return services[generator.nextInt(services.length)].name;
    }
}
