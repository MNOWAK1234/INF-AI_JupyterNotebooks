package exercise1;

public class Weather 
{
    private final Forecast forecast;
    private final float temperature;
    private final char scale;
    private final String description;

    public Weather(Forecast forecast, float temperature, char measurementType, String description)
    {
        this.forecast=forecast;
        this.temperature=temperature;
        this.scale=measurementType;
        this.description=description;
    }
    public Weather DifferentScale(char scale)
    {
        if(this.scale=='K' && scale=='C')
        {
            float scienceTemperature=this.temperature-273.15f;
            return new Weather(this.forecast, scienceTemperature, scale, this.description);
        }
        else if(this.scale=='C' && scale=='K')
        {
            float casualTemperature=this.temperature+273.15f;
            return new Weather(this.forecast, casualTemperature, scale, this.description);
        }
        else if(this.scale=='C' && scale=='F')
        {
            float casualTemperature=this.temperature*1.8f+32.f;
            return new Weather(this.forecast, casualTemperature, scale, this.description);
        }
        else return new Weather(this.forecast, this.temperature, this.scale, this.description);
    }
    public Weather LowestPossible(char scale)
    {
        float newTemperature=this.temperature/this.forecast.errorMargin(this.scale);
        return new Weather(this.forecast, newTemperature, this.scale, this.description);
    }

    public float deviation()
    {
        float deviationSquared=this.temperature;
        if(this.scale=='K')deviationSquared-=290;
        else if(this.scale=='C')deviationSquared-=20;
        if(this.scale=='F')deviationSquared-=70;
        return deviationSquared*deviationSquared;
    }
}