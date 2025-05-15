package pl.poznan.put.content;

import java.util.Vector;

public class FootballLeague {
  private Vector<Team>contestants = new Vector<>();
  private Vector<Integer>pairing = new Vector<>();
  private String description;
  public FootballLeague(String stage, Vector<Team>teams) {
    this.description = stage;
    this.contestants = teams;
  }
  public Vector<Integer> generatePairing() {
    if(this.description.equals("Group")) {
      this.pairing.add(-1);
      for(int i = 0; i < this.contestants.size(); i++) {
        for(int j = 0 ; j < this.contestants.size(); j++) {
          if(i <= j) {
            this.pairing.add(i);
            this.pairing.add(j);
          }
        }
      }
    }
    else if(this.description.equals("League")) {
      this.pairing.add(-2);
      for(int i = 0; i < this.contestants.size(); i++) {
        for(int j = 0; j < this.contestants.size(); j++) {
          if(i != j) {
            this.pairing.add(j);
            this.pairing.add(i);
          }
        }
      }
    }
    return this.pairing;
  }
}
