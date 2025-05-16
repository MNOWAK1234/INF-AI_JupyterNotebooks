package edu.put.inf151813

import android.content.Intent
import androidx.appcompat.app.AppCompatActivity
import android.os.Bundle
import android.widget.Button

class MainActivity : AppCompatActivity() {
    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        setContentView(R.layout.activity_main)

        val dateActivity = findViewById<Button>(R.id.data)
        dateActivity.setOnClickListener{
            val intent1 = Intent (this, Data::class.java)
            startActivity(intent1)
        }

        val timeActivity = findViewById<Button>(R.id.czas)
        timeActivity.setOnClickListener{
            val intent2 = Intent (this, Czas::class.java)
            startActivity(intent2)
        }
    }
}