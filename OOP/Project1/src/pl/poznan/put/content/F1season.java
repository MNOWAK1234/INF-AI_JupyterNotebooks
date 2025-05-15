package pl.poznan.put.content;

import java.util.Vector;
import java.util.Random;

public class F1season {
    private Vector<Integer>races = new Vector<>();
    private int calendar;
    public F1season() {
        Random r = new Random();
        this.calendar = r.nextInt(5) + 18;
        for(int i = 0; i < this.calendar; i++) {
            this.races.add(r.nextInt(10)+1);
        }
    }
    public Vector<Integer> publishCalendar() {
        return this.races;
    }
}
