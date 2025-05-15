package pl.poznan.put.content;

import java.util.Vector;
import java.util.Collections;

public class Prepare implements IPrepare {
    public Vector<Integer> prepare(String sport) {
        PointsSystem points = new PointsSystem(sport);
        Vector<Integer>scoringSheet = points.scorage();
        return scoringSheet;
    }
    public Vector<Team> prepareResults(Vector<Team> teams) {
        Collections.sort(teams, (t1, t2) -> t2.tally() - t1.tally());
        return teams;
    }
}
