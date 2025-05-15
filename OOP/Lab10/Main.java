public class Main {
    public static void main(String[] args) {
        Sequence seq = new Substring(new Concaternation(
            new LowerCased(new Characters("WOULD IT EVER WORK")),
            new LowerCased(new Characters("S EVENTUALLY!"))
            ), 14, 30);
        String string = seq.printedSeq();
        System.out.println(string);
    }
}
