package pl.poznan.put.content;

import java.util.Vector;

public class Simulation {
    private String sport;
    private Vector<Team>teams = new Vector<>();
    private Vector<Integer> scoringSheet = new Vector<>();
    private IFormula1 F1;
    private ITeamSport League;
    private ICompetition Cup;
    private IPrepare Results;
    private IDisplay Table;
    public Simulation(String sport, Vector<Team> teams){
        this.sport = sport;
        this.teams = teams;
        this.F1 = new Formula1();
        this.League = new TeamSport();
        this.Cup = new Competition();
        this.Results = new Prepare();
        this.Table = new Display();
    }
    public Vector<Integer> prepare(String sport) {
        return Results.prepare(sport);
    }
    public Vector<Team> prepareResults(Vector<Team> teams){
        return Results.prepareResults(teams);
    }
    public void showTeam(Team one){
        Table.showTeam(one);
    }
    public void readResults(Vector<Team> teams, String sport){
        Table.readResults(teams, sport);
    }
    public Vector<Team> wholeSeason(Vector<Team> teams, Vector<Integer> scoringSheet){
        return F1.wholeSeason(teams, scoringSheet);
    }
    public Vector<Team> wholeLeague(Vector<Team> teams, Vector<Integer> scoringSheet){
        return League.wholeLeague(teams, scoringSheet);
    }
    public Vector<Team> roadToGlory(Vector<Team> teams, Vector<Integer> scoringSheet){
        return Cup.roadToGlory(teams, scoringSheet);
    }
    public void simulate() {
        this.scoringSheet=prepare(this.sport);
        if(this.sport.equals("Football")) {
            this.teams=wholeLeague(this.teams, this.scoringSheet);
        }
        else if(this.sport.equals("F1")) {
            this.teams=wholeSeason(this.teams, this.scoringSheet);
        }
        else if(this.sport.equals("Cup")) {
            this.teams=roadToGlory(this.teams, this.scoringSheet);
        }
        this.prepareResults(this.teams);
    }
}
