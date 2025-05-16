const rp = require("request-promise");
const cheerio = require("cheerio");
const fs = require("fs");

// Adres URL strony, którą chcemy przeszukać
const url = "https://gratka.pl/nieruchomosci/mieszkania/poznan";

// Opcje dla request-promise
const options = {
  uri: url,
  transform: function (body) {
    return cheerio.load(body);
  },
};

// Funkcja do pobrania i przetworzenia danych
async function crawlWebsite() {
  try {
    const $ = await rp(options);
    const apartments = [];

    // Przechodzimy przez każdy artykuł na stronie
    $(".teaserUnified").each((index, element) => {
      const params = $(element)
        .find(".teaserUnified__params")
        .text()
        .trim()
        .split("\n");
      let area = 0;
      let rooms = "Brak danych";
      let floor = "Brak danych";

      if (params.length >= 1) {
        area = parseFloat(
          params[0].trim().replace(",", ".").replace(" m²", "")
        );
        if (params.length >= 3) {
          rooms = parseInt(params[1].trim()) || "Brak danych";
          floor = params[2].trim() || "Brak danych";
        }
      }
      /*const area = parseFloat(
        params[0].trim().replace(",", ".").replace(" m²", "")
      );
      const rooms = parseInt(params[1].trim());
      const floor = params[2].trim();*/

      const priceElement = $(element).find(".teaserUnified__price");
      const price = parseInt(
        priceElement.contents().first().text().trim().replace(/\s+/g, "")
      );

      const apartmentName = $(element)
        .find(".teaserUnified__title")
        .text()
        .trim();

      const pricePerSquareMeter = price / area;

      const score = pricePerSquareMeter;

      apartments.push({
        area: area,
        rooms: rooms,
        floor: floor,
        price: price,
        apartmentName: apartmentName,
        score: score,
      });
    });

    // Sortujemy mieszkania według wyznacznika score
    const sortedApartments = apartments.sort((a, b) => a.score - b.score);

    // Wyświetlamy posortowane mieszkania
    sortedApartments.forEach((apartment) => {
      console.log("Mieszkanie:", apartment.apartmentName);
      console.log("Metraż:", apartment.area + " m2");
      console.log("Liczba pokoi:", apartment.rooms);
      console.log("Piętro:", apartment.floor);
      console.log("Cena:", apartment.price + " zł");
      console.log("Cena za m2:", apartment.score); // Dodajemy wypisanie wartości sortującej
      console.log("---");
    });

    // Zapisujemy posortowane mieszkania do pliku
    const dataToWrite = sortedApartments.map((apartment) => {
      return `Mieszkanie: ${apartment.apartmentName}\nMetraż: ${apartment.area} m2\nLiczba pokoi: ${apartment.rooms}\nPiętro: ${apartment.floor}\nCena: ${apartment.price} zł\nCena za m2: ${apartment.score}\n---\n`;
    });
    fs.writeFileSync("sorted_apartments.txt", dataToWrite.join(""), "utf-8");
    console.log("Dane zostały zapisane do pliku sorted_apartments.txt");
  } catch (error) {
    console.error("Wystąpił błąd:", error);
  }
}

// Wywołanie funkcji do przetwarzania strony
crawlWebsite();
