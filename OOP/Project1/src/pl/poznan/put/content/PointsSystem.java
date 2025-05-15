package pl.poznan.put.content;

import java.util.Vector;

public class PointsSystem {
    private String sport;
    private Vector<Integer>points = new Vector<>();
    public PointsSystem(String sport) {
        this.sport = sport;
        if(this.sport.equals("F1")) {
            this.points.add(25);
            this.points.add(18);
            this.points.add(15);
            this.points.add(12);
            this.points.add(10);
            this.points.add(8);
            this.points.add(6);
            this.points.add(4);
            this.points.add(2);
            this.points.add(1);
            this.points.add(0);
            this.points.add(0);
            this.points.add(0);
            this.points.add(0);
            this.points.add(0);
            this.points.add(0);
            this.points.add(0);
            this.points.add(0);
            this.points.add(0);
            this.points.add(0);
        }
        else {
            this.points.add(3);
            this.points.add(1);
            this.points.add(0);
        }
    }
    public Vector<Integer> scorage() {
        return this.points;
    }
}
