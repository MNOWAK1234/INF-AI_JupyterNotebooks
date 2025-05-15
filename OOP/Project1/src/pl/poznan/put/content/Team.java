package pl.poznan.put.content;

public class Team {
    String name;
    int points;
    int ovr;
    String place;
  
    public Team(String name, int ovr) {
      this.name = name;
      this.points = 0;
      this.ovr = ovr;
      this.place = "";
    }
    public Team(int ovr){
      this.name = "Artificial";
      this.points = 0;
      this.ovr = ovr;
      this.place = "";
    }
    public void printStats(){
      System.out.println(this.name + " " + this.ovr);
    }
    public void achievePoints(int p) {
      this.points += p;
    }
    public void result(String res) {
      this.place = res;
    }
    public int strength() {
      return this.ovr;
    }
    public String tellName() {
      return this.name;
    }
    public int tally() {
      return this.points;
    }
    public String stage() {
      return this.place;
    }
}