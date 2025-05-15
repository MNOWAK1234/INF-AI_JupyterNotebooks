public class Characters implements Sequence{
    private String s;

    public Characters(String s) {
        this.s = s;
    }

    @Override
    public String printedSeq() {
        return this.s;
    }
}
