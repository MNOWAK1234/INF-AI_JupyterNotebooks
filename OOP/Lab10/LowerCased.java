public class LowerCased implements Sequence{
    private Sequence s;

    public LowerCased(Sequence s) {
        this.s = s;
    }

    @Override
    public String printedSeq() {
        return s.printedSeq().toLowerCase();
    }
}
