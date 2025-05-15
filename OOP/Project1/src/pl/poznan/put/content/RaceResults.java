package pl.poznan.put.content;

public class RaceResults {
    private int id;
    private int performance;
    public RaceResults(int id, int performance) {
        this.id = id;
        this.performance = performance;
    }
    public int racingNumber() {
        return this.id;
    }
    public int pace() {
        return this.performance;
    }
}
