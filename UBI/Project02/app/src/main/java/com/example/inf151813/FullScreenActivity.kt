package com.example.inf151813

import android.net.Uri
import android.os.Bundle
import android.widget.ImageView
import androidx.appcompat.app.AppCompatActivity

class FullScreenActivity : AppCompatActivity() {

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        setContentView(R.layout.full_screen)

        val imageView = findViewById<ImageView>(R.id.fullScreenImageView)

        val imageUri = intent.getStringExtra("imageUri")
        if (imageUri != null) {
            val uri = Uri.parse(imageUri)
            imageView.setImageURI(uri)
        }
    }
}
