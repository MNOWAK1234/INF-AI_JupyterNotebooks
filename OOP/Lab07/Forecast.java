package exercise1;

public interface Forecast 
{
    float errorMargin(char scaleSymbol);
    final class Fake implements Forecast
    {
        public float errorMargin(char scaleSymbol)
        {
            if(scaleSymbol=='C')return 1.03f;
            else if(scaleSymbol=='K')return 1.001f;
            else if(scaleSymbol=='F')return 1.7f;
            else return 1000.f;
        }
    }
}