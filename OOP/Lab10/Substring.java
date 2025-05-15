public class Substring implements Sequence{
    private Sequence s;
    private int begin;
    private int end;

    public Substring(Sequence s, int begin, int end) {
        this.s = s;
        this.begin = begin;
        this.end = end;
    }

    @Override
    public String printedSeq() {
        return this.s.printedSeq().substring(begin, end);
    }
}
