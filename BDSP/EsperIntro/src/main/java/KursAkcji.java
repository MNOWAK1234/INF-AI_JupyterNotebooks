import java.text.SimpleDateFormat;
import java.util.Date;

public class KursAkcji {
	private String spolka;
	private String market;
	private Date data;
	private Float kursOtwarcia;
	private Float wartoscMax;
	private Float wartoscMin;
	private Float kursZamkniecia;
	private Float obrot;

	private static final SimpleDateFormat df = new SimpleDateFormat("yyyy-MM-dd");
	
	public KursAkcji(String spolka, String market, Date data,
			Float kursOtwarcia, Float wartoscMax, Float wartoscMin,
			Float kursZamkniecia, Float obrot) {
		this.spolka = spolka;
		this.market = market;
		this.data = data;
		this.kursOtwarcia = kursOtwarcia;
		this.wartoscMax = wartoscMax;
		this.wartoscMin = wartoscMin;
		this.kursZamkniecia = kursZamkniecia;
		this.obrot = obrot;
	}

	@Override
	public String toString() {
		return "KursAkcji   [spolka=" + spolka + ",\tmarket=" + market
				+ ",\tdata=" + df.format(data) + ",\tkursOtwarcia=" + kursOtwarcia
				+ ",\twartoscMax=" + wartoscMax + ",\twartoscMin=" + wartoscMin
				+ ",\tkursZamkniecia=" + kursZamkniecia + ",\tobrot=" + obrot
				+ "]";
	}

	public String getSpolka() {
		return spolka;
	}

	public void setSpolka(String spolka) {
		this.spolka = spolka;
	}

	public String getMarket() {
		return market;
	}

	public void setMarket(String market) {
		this.market = market;
	}

	public Date getData() {
		return data;
	}

	public void setData(Date data) {
		this.data = data;
	}

	public Float getKursOtwarcia() {
		return kursOtwarcia;
	}

	public void setKursOtwarcia(Float kursOtwarcia) {
		this.kursOtwarcia = kursOtwarcia;
	}

	public Float getWartoscMax() {
		return wartoscMax;
	}

	public void setWartoscMax(Float wartoscMax) {
		this.wartoscMax = wartoscMax;
	}

	public Float getWartoscMin() {
		return wartoscMin;
	}

	public void setWartoscMin(Float wartoscMin) {
		this.wartoscMin = wartoscMin;
	}

	public Float getKursZamkniecia() {
		return kursZamkniecia;
	}

	public void setKursZamkniecia(Float kursZamkniecia) {
		this.kursZamkniecia = kursZamkniecia;
	}

	public Float getObrot() {
		return obrot;
	}

	public void setObrot(Float obrot) {
		this.obrot = obrot;
	}
}
