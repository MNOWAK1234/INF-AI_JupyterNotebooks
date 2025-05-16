package edu.put.inf151813

import androidx.appcompat.app.AppCompatActivity
import android.os.Bundle
import android.widget.Button
import android.widget.TextView
import java.math.BigInteger

class Czas : AppCompatActivity() {
    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        setContentView(R.layout.activity_czas)

        val plus = findViewById<Button>(R.id.plus)
        val minus = findViewById<Button>(R.id.minus)
        val ac = findViewById<Button>(R.id.clear)
        val hours1: TextView = findViewById(R.id.h1u)
        val hours2: TextView = findViewById(R.id.h2u)
        val minutes1: TextView = findViewById(R.id.m1u)
        val minutes2: TextView = findViewById(R.id.m2u)
        val seconds1: TextView = findViewById(R.id.s1u)
        val seconds2: TextView = findViewById(R.id.s2u)

        var h1 = 0
        var h2 = 0
        var m1 = 0
        var m2 = 0
        var s1 = 0
        var s2 = 0
        var t1: BigInteger = BigInteger.valueOf((s1 + m1*60 + h1*3600).toLong())
        var t2: BigInteger = BigInteger.valueOf((s2 + m2*60 + h2*3600).toLong())
        var sign = 1

        plus.setOnClickListener(){
            s1 = seconds1.text.toString().toIntOrNull() ?: 0
            s2 = seconds2.text.toString().toIntOrNull() ?: 0
            m1 = minutes1.text.toString().toIntOrNull() ?: 0
            m2 = minutes2.text.toString().toIntOrNull() ?: 0
            h1 = hours1.text.toString().toIntOrNull() ?: 0
            h2 = hours2.text.toString().toIntOrNull() ?: 0
            t1 = BigInteger.valueOf((s1 + m1*60 + h1*3600).toLong())
            t2 = BigInteger.valueOf((s2 + m2*60 + h2*3600).toLong())

            var sum = t1.add(t2)

            if(sum >= BigInteger.valueOf(0)) {
                sign = 1
            }
            else {
                sign = -1
                sum = sum.negate()
            }

            s1 = (sum % BigInteger("60")).toInt()
            sum = sum.divide(BigInteger.valueOf(60))
            m1 = (sum % BigInteger("60")).toInt()
            sum = sum.divide(BigInteger.valueOf(60))
            h1 = sum.toInt()
            if(sign == -1){
                s1 *= -1
                m1 *= -1
                h1 *= -1
            }
            s2 = 0
            m2 = 0
            h2 = 0

            seconds1.text = "$s1"
            seconds2.text = "$s2"
            minutes1.text = "$m1"
            minutes2.text = "$m2"
            hours1.text = "$h1"
            hours2.text = "$h2"
        }
        minus.setOnClickListener(){
            s1 = seconds1.text.toString().toIntOrNull() ?: 0
            s2 = seconds2.text.toString().toIntOrNull() ?: 0
            m1 = minutes1.text.toString().toIntOrNull() ?: 0
            m2 = minutes2.text.toString().toIntOrNull() ?: 0
            h1 = hours1.text.toString().toIntOrNull() ?: 0
            h2 = hours2.text.toString().toIntOrNull() ?: 0
            t1 = BigInteger.valueOf((s1 + m1*60 + h1*3600).toLong())
            t2 = BigInteger.valueOf((s2 + m2*60 + h2*3600).toLong())

            var difference = t1.subtract(t2)

            if(difference >= BigInteger.valueOf(0)) {
                sign = 1
            }
            else {
                sign = -1
                difference = difference.negate()
            }

            s1 = (difference % BigInteger("60")).toInt()
            difference = difference.divide(BigInteger.valueOf(60))
            m1 = (difference % BigInteger("60")).toInt()
            difference = difference.divide(BigInteger.valueOf(60))
            h1 = difference.toInt()
            if(sign == -1){
                s1 *= -1
                m1 *= -1
                h1 *= -1
            }
            s2 = 0
            m2 = 0
            h2 = 0

            seconds1.text = "$s1"
            seconds2.text = "$s2"
            minutes1.text = "$m1"
            minutes2.text = "$m2"
            hours1.text = "$h1"
            hours2.text = "$h2"
        }
        ac.setOnClickListener(){
            seconds1.text = "0"
            seconds2.text = "0"
            minutes1.text = "0"
            minutes2.text = "0"
            hours1.text = "0"
            hours2.text = "0"
        }
    }
}