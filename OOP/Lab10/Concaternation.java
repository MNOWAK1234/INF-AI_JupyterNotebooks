public class Concaternation implements Sequence{

    private Sequence a;
    private Sequence b;

    public Concaternation(Sequence a, Sequence b) {
        this.a = a;
        this.b = b;
    }

    @Override
    public String printedSeq() {
        return this.a.printedSeq() + this.b.printedSeq();
    }

}
