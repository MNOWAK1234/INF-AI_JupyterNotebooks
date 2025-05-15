package exercise1;

public class Main 
{
    public static void main(String[] args)
    {
        Forecast forecast=new Forecast.Fake();
        //try to change the temperature and check if Fahrenheit/Kelvin/Celsius deviates too much
        Weather today=new Weather(forecast, 28.5f, 'C', "sunny");
        assert today.deviation()<=300;
        System.out.print("Initialisation OK\n");
        Weather tonight=today.LowestPossible('C');
        assert tonight.deviation()<=300;
        System.out.print("Changed OK\n");
        Weather US=today.DifferentScale('F');
        assert US.deviation()<=300;
        System.out.print("USA OK\n");
        Weather Science=today.DifferentScale('K');
        assert Science.deviation()<=300;
        System.out.print("Science OK\n");
        System.out.print("Everything OK");
    }
}