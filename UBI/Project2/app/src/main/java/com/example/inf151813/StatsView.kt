package com.example.inf151813

import android.app.Activity
import android.content.Intent
import android.graphics.Bitmap
import android.graphics.BitmapFactory
import android.graphics.Typeface
import android.net.Uri
import androidx.appcompat.app.AppCompatActivity
import android.os.Bundle
import android.os.Handler
import android.os.Looper
import android.provider.MediaStore
import android.view.Gravity
import android.view.LayoutInflater
import android.view.View
import android.view.ViewGroup
import android.widget.Button
import android.widget.ImageView
import android.widget.LinearLayout
import android.widget.TableLayout
import android.widget.TableRow
import android.widget.TextView
import androidx.core.content.ContextCompat
import androidx.recyclerview.widget.LinearLayoutManager
import androidx.recyclerview.widget.RecyclerView
import java.security.KeyStore
import java.util.concurrent.Executors

class StatsView : AppCompatActivity() {
    var Id: Int = 0
    private lateinit var imageView: ImageView
    private lateinit var selectedImageUris: MutableList<Uri>
    private lateinit var photoContainer: LinearLayout
    private lateinit var addButton: Button
    companion object {
        private const val PICK_IMAGE_REQUEST = 1
    }

    private val photoList: ArrayList<Uri> = ArrayList()

    private lateinit var recyclerView: RecyclerView
    private lateinit var adapter: PhotoAdapter

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        setContentView(R.layout.activity_stats_view)
        val extras = intent.extras
        Id = extras!!.getInt("Id")
        val dbm = DBHandlerMain(this, null, null, 1)
        val rec = dbm.findRecord(Id)
        //setCaptions(rec!!)
        recyclerView = findViewById(R.id.recyclerView)
        adapter = PhotoAdapter(photoList)
        recyclerView.adapter = adapter
        recyclerView.layoutManager = LinearLayoutManager(this)

        populate()

    }

    inner class PhotoAdapter(private val photoList: List<Uri>) : RecyclerView.Adapter<PhotoAdapter.PhotoViewHolder>() {

        override fun onCreateViewHolder(parent: ViewGroup, viewType: Int): PhotoViewHolder {
            val view = LayoutInflater.from(parent.context).inflate(R.layout.item_photo, parent, false)
            return PhotoViewHolder(view)
        }

        override fun onBindViewHolder(holder: PhotoViewHolder, position: Int) {
            val photoUri = photoList[position]
            holder.bind(photoUri)
        }

        override fun getItemCount(): Int {
            return photoList.size
        }

        inner class PhotoViewHolder(itemView: View) : RecyclerView.ViewHolder(itemView) {
            private val imageView: ImageView = itemView.findViewById(R.id.imageView)

            fun bind(photoUri: Uri) {
                imageView.setImageURI(photoUri)

                imageView.setOnClickListener {
                    val intent = Intent(itemView.context, FullScreenActivity::class.java)
                    intent.putExtra("imageUri", photoUri.toString())
                    itemView.context.startActivity(intent)
                }
            }
        }
    }


    fun onAddPhotoClick(view: View) {
        openGallery()
    }

    fun deleteAllPhotos(view: View) {
        photoList.clear()
        adapter.notifyDataSetChanged()
    }

    private fun openGallery() {
        val intent = Intent(Intent.ACTION_GET_CONTENT)
        intent.type = "image/*"
        intent.putExtra(Intent.EXTRA_ALLOW_MULTIPLE, true)
        startActivityForResult(Intent.createChooser(intent, "Select Photos"), PICK_IMAGE_REQUEST)
    }

    override fun onActivityResult(requestCode: Int, resultCode: Int, data: Intent?) {
        super.onActivityResult(requestCode, resultCode, data)

        if (requestCode == PICK_IMAGE_REQUEST && resultCode == Activity.RESULT_OK && data != null) {
            if (data.clipData != null) {
                val count = data.clipData!!.itemCount
                for (i in 0 until count) {
                    val imageUri = data.clipData!!.getItemAt(i).uri
                    photoList.add(imageUri)
                }
            } else if (data.data != null) {
                val imageUri = data.data
                if (imageUri != null) {
                    photoList.add(imageUri)
                }
            }

            adapter.notifyDataSetChanged()
        }
    }

    private fun populate() {
        val dbs = DBHandlerStat(this, null, null, 1)
        val table: TableLayout = findViewById(R.id.tblStats)
        val theList = dbs.getStats(Id)
        for (i in 0..theList.size - 1) {
            val row = TableRow(this)
            val lp = TableRow.LayoutParams(TableRow.LayoutParams.WRAP_CONTENT)
            row.layoutParams = lp
            row.gravity = Gravity.CENTER_HORIZONTAL
            row.setPadding(0, 10, 0, 10)
            val textViewDate = TextView(this)
            textViewDate.text = theList[i].date_of_sync
            textViewDate.setPadding(10, 0, 10, 0)
            textViewDate.textSize = 16F
            textViewDate.textAlignment = View.TEXT_ALIGNMENT_CENTER
            if (i == 0) {
                textViewDate.setTypeface(null, Typeface.BOLD)
            }
            val textViewPos = TextView(this)
            textViewPos.text = theList[i].position.toString()
            textViewPos.setPadding(10, 0, 10, 0)
            textViewPos.textSize = 16F
            textViewPos.textAlignment = View.TEXT_ALIGNMENT_CENTER
            if (i == 0) {
                textViewPos.setTypeface(null, Typeface.BOLD)
            }
            row.addView(textViewDate)
            row.addView(textViewPos)
            table.addView(row, i + 1)
        }
    }
}