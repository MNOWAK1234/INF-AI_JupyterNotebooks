package edu.put.inf151813

import androidx.appcompat.app.AppCompatActivity
import android.os.Bundle
import android.widget.Button
import android.widget.DatePicker
import android.widget.TextView
import java.time.temporal.ChronoUnit
import java.util.Calendar
import java.time.DayOfWeek
import java.time.LocalDate

class Data : AppCompatActivity() {

    fun getEasterSunday(date: LocalDate): LocalDate {
        val year = date.year
        val a = year % 19
        val b = year / 100
        val c = year % 100
        val d = b / 4
        val e = b % 4
        val f = (b + 8) / 25
        val g = (b - f + 1) / 3
        val h = (19 * a + b - d - g + 15) % 30
        val i = c / 4
        val k = c % 4
        val l = (32 + 2 * e + 2 * i - h - k) % 7
        val m = (a + 11 * h + 22 * l) / 451
        val month = (h + l - 7 * m + 114) / 31
        val day = ((h + l - 7 * m + 114) % 31) + 1

        return LocalDate.of(year, month, day)
    }

    fun countWorkdays(startDate: LocalDate, endDate: LocalDate): Int {
        var count = 0
        var currentDate = startDate
        var workday = 1
        while (currentDate < endDate) {
            workday = 1
            if (currentDate.dayOfWeek == DayOfWeek.SATURDAY || currentDate.dayOfWeek == DayOfWeek.SUNDAY) {
                workday = 0
            }
            if(currentDate.monthValue == 1 && currentDate.dayOfMonth == 1){
                workday = 0
            }
            if(currentDate.monthValue == 1 && currentDate.dayOfMonth == 6){
                workday = 0
            }
            if(currentDate.monthValue == 5 && currentDate.dayOfMonth == 1){
                workday = 0
            }
            if(currentDate.monthValue == 5 && currentDate.dayOfMonth == 3){
                workday = 0
            }
            if(currentDate.monthValue == 8 && currentDate.dayOfMonth == 15){
                workday = 0
            }
            if(currentDate.monthValue == 11 && currentDate.dayOfMonth == 1){
                workday = 0
            }
            if(currentDate.monthValue == 11 && currentDate.dayOfMonth == 11){
                workday = 0
            }
            if(currentDate.monthValue == 12 && currentDate.dayOfMonth == 25){
                workday = 0
            }
            if(currentDate.monthValue == 12 && currentDate.dayOfMonth == 26){
                workday = 0
            }
            var Easter: LocalDate = getEasterSunday(currentDate)
            var EasterMonday: LocalDate = Easter.plusDays(1)
            var CorpusChristi: LocalDate = Easter.plusDays(60)
            if(currentDate == EasterMonday || currentDate == CorpusChristi){
                workday = 0
            }
            count += workday
            currentDate = currentDate.plusDays(1)
        }

        return count
    }

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        setContentView(R.layout.activity_data)

        val datePicker1: DatePicker = findViewById(R.id.firstDatePicker)
        val status: TextView = findViewById(R.id.dodanie)
        val datePicker2: DatePicker = findViewById(R.id.secondDatePicker)
        val oblicz = findViewById<Button>(R.id.obliczenia)
        val praca: TextView = findViewById(R.id.robocze)
        var daysDiff: Long = 0
        var dniRobocze: Long = 0
        datePicker1.setOnDateChangedListener() {
            view, year, monthOfYear, dayOfMonth ->
            val startDate = LocalDate.of(datePicker1.year, datePicker1.month + 1, datePicker1.dayOfMonth)
            val endDate = LocalDate.of(datePicker2.year, datePicker2.month + 1, datePicker2.dayOfMonth)
            daysDiff = ChronoUnit.DAYS.between(startDate, endDate)
            status.text = "$daysDiff"
            if(daysDiff>=0){
                dniRobocze = countWorkdays(startDate, endDate).toLong()
            }
            else{
                dniRobocze = countWorkdays(endDate, startDate).toLong()
                dniRobocze *= -1
            }
            praca.text = "$dniRobocze"
        }
        datePicker2.setOnDateChangedListener() {
                view, year, monthOfYear, dayOfMonth ->
            val startDate = LocalDate.of(datePicker1.year, datePicker1.month + 1, datePicker1.dayOfMonth)
            val endDate = LocalDate.of(datePicker2.year, datePicker2.month + 1, datePicker2.dayOfMonth)
            daysDiff = ChronoUnit.DAYS.between(startDate, endDate)
            status.text = "$daysDiff"
            if(daysDiff>=0){
                dniRobocze = countWorkdays(startDate, endDate).toLong()
            }
            else{
                dniRobocze = countWorkdays(endDate, startDate).toLong()
                dniRobocze *= -1
            }
            praca.text = "$dniRobocze"
        }
        oblicz.setOnClickListener() {
            val calendar = Calendar.getInstance()
            calendar.set(datePicker1.year, datePicker1.month, datePicker1.dayOfMonth)
            calendar.add(Calendar.DAY_OF_YEAR, status.text.toString().toIntOrNull() ?: 0)
            daysDiff = (status.text.toString().toIntOrNull() ?: 0).toLong()
            datePicker2.updateDate(calendar.get(Calendar.YEAR), calendar.get(Calendar.MONTH), calendar.get(Calendar.DAY_OF_MONTH))
            val startDate = LocalDate.of(datePicker1.year, datePicker1.month + 1, datePicker1.dayOfMonth)
            val endDate = LocalDate.of(datePicker2.year, datePicker2.month + 1, datePicker2.dayOfMonth)
            if(daysDiff>=0){
                dniRobocze = countWorkdays(startDate, endDate).toLong()
            }
            else{
                dniRobocze = countWorkdays(endDate, startDate).toLong()
                dniRobocze *= -1
            }
            praca.text = "$dniRobocze"
        }
    }
}