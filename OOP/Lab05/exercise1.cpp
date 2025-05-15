///cannot check if my code compile because of too old c++ version

#include <iostream>
#include <map>
#include <sstream>
#include <iomanip>

using namespace std;

class FakeCantor {
 private:
  std::map<std::string, float> rates{{"USD", 1.0366f},
                                     {"GBP", 0.87063f},
                                     {"CHF", 0.9881f},
                                     {"JPY", 145.12f}};
  public:
  FakeCantor() = default;
  float EuroToRate(const std::string &currency) {
    return rates[currency];
  };
};

class Currency {
 public:
  virtual Currency* AddedCurrency(float value, std::string currency) = 0;
  virtual Currency* SubtractedCurrency(float value, std::string currency) = 0;
  virtual std::string Abbreviation() = 0;
  virtual std::string Symbol() = 0;
  virtual std::string Balance() = 0;
  virtual float DollarExchangeRate() = 0;
};

class Euro : public Currency
{
private:
    float value;
public:
    Euro(float value)
    {
        this->value=value;
    }
    Currency* AddedCurrency(float howMuch, string e)
    {
        Currency* nxt = new Currency(value+howMuch, e);
        return nxt;
    }
    Currency* SubtractedCurrency(float howMuch, string e)
    {
        if(value-howMuch>=0)Currency* nxt = new Currency(value-howMuch, e);
        else Currency* nxt = new Currency(0, e);
        return nxt;
    }
    string Abbreviation()
    {
        return "EUR";
    }
    string Symbol()
    {
        return "€";
    }
    string Balance()
    {
        std::ostringstream ss;
        ss<<value;
        std::string s(ss.str());
        return s+"€";
    }
    float DollarExchangeRate()
    {
        return FakeCantor.EuroToRate("USD");
    }


};

int main()
{
 FakeCantor MyCantor;
  float rate=MyCantor.EuroToRate("USD");
  Euro EuroCurrency(rate);
  std::cout<<EuroCurrency.Abbreviation()<<std::endl;
  std::cout<<EuroCurrency.Symbol()<<std::endl;
  std::cout<<EuroCurrency.Balance()<<std::endl;
  std::cout<<EuroCurrency.DollarExchangeRate()<<std::endl;
 return 0;
}
