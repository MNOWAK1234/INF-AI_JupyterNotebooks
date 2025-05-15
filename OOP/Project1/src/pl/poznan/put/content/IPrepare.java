package pl.poznan.put.content;

import java.util.Vector;

public interface IPrepare {
    public Vector<Integer> prepare(String Sport);
    public Vector<Team> prepareResults(Vector<Team> teams);
}
