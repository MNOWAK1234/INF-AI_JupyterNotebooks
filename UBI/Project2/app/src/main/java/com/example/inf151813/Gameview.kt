package com.example.inf151813

import android.app.Activity
import android.content.Intent
import android.graphics.Bitmap
import android.graphics.BitmapFactory
import android.os.Bundle
import android.os.Handler
import android.os.Looper
import android.view.Gravity
import android.view.View
import android.widget.ImageView
import android.widget.TableLayout
import android.widget.TableRow
import android.widget.TextView
import androidx.appcompat.app.AlertDialog
import androidx.appcompat.app.AppCompatActivity
import java.util.concurrent.Executors


class GameView : AppCompatActivity() {
    var addons: Boolean = false
    var desc = false
    var actO = Orders._ID

    fun Boolean.toInt() = if (this) 1 else 0
    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        setContentView(R.layout.activity_game_view)
        val extras = intent.extras
        val addons = extras?.getBoolean("Additions", false) ?: false
        var textView: TextView = findViewById(R.id.textView)
        if(addons==true){
            textView.text = "Additons"
        }
        else{
            textView.text = "Games"
        }
        val dbHandler = DBHandlerMain(this, null, null,  1)
        val theList = dbHandler.getVals(addons.toInt(), actO, desc)
        makeTable(theList)
    }
    fun stringCutter(src: String):String{
        var res: String = ""
        var lim = 10
        for(i in 0..src.length-1){
            if(src[i] != ' '){
                res += src[i]
            }
            else{
                if(i >= lim){
                    res+= '\n'
                    lim+=i
                }
                else{
                    res+=' '
                }
            }
        }
        return res
    }

    fun makeTable(l: List<Record>) {
        // Get a reference to the TableLayout
        val table: TableLayout = findViewById(R.id.tblLayout)

        // Iterate through the list of records
        for (i in 0..l.lastIndex) {
            // Create a new TableRow
            val row = TableRow(this)

            // Set layout parameters for the TableRow
            val lp = TableRow.LayoutParams(TableRow.LayoutParams.WRAP_CONTENT)
            row.layoutParams = lp

            // Create TextViews to display the record's ID, title, and year
            val textViewId = TextView(this)
            textViewId.text = l[i].id.toString()
            textViewId.setPadding(20, 15, 20, 15)

            val textViewName = TextView(this)
            textViewName.text = stringCutter(l[i].title!!)
            textViewName.setPadding(20, 15, 20, 15)

            val textViewYear = TextView(this)
            textViewYear.text = l[i].year_pub.toString()
            textViewYear.setPadding(20, 15, 20, 15)

            // Create an ImageView to display the record's image
            val im = ImageView(this)
            // Execute image loading in a separate thread
            val executor = Executors.newSingleThreadExecutor()
            val handler = Handler(Looper.getMainLooper())
            var image: Bitmap? = null
            executor.execute {
                val imageURL = l[i].pic
                try {
                    // Load the image from the specified URL
                    val `in` = java.net.URL(imageURL).openStream()
                    image = BitmapFactory.decodeStream(`in`)
                    // Update the ImageView with the loaded image on the main thread
                    handler.post {
                        im.setImageBitmap(image)
                    }
                } catch (e: Exception) {
                    // If an error occurs while loading the image, use a default image
                    image = BitmapFactory.decodeResource(
                        this.resources,
                        R.drawable.rectangular_button
                    )
                    im.setImageBitmap(image)
                    e.printStackTrace()
                }
            }

            // Add the TextViews and ImageView to the TableRow
            row.addView(textViewId)
            row.addView(textViewName)
            row.addView(textViewYear)
            row.addView(im)

            // Add an additional TextView for rank if not in "addons" mode
            if (!addons) {
                val textViewRank = TextView(this)
                if (l[i].rank_pos == 0) {
                    textViewRank.text = "N/A"
                } else {
                    textViewRank.text = l[i].rank_pos.toString()
                    // Set an onClickListener to perform an action when the row is clicked
                    row.setOnClickListener {
                        callStats(l[i].id)
                    }
                }
                textViewRank.setPadding(20, 15, 20, 15)
                textViewRank.gravity = Gravity.CENTER
                row.addView(textViewRank)
            } else {
                // Hide the rank caption if in "addons" mode
                val rankCap: TextView = findViewById(R.id.RANKtv)
                rankCap.visibility = View.INVISIBLE
            }

            // Add the populated TableRow to the TableLayout
            table.addView(row, i)
        }
    }

    fun callStats(id: Int) {
        val i = Intent(this, StatsView::class.java)
        val b = Bundle()
        b.putInt("Id", id)
        i.putExtras(b)
        startActivity(i)
    }

    fun clear(){
        val table: TableLayout = findViewById(R.id.tblLayout)
        val childCount = table.childCount
        if (childCount > 0) {
            table.removeViews(0, childCount)
        }
    }
    fun checkSet(new: Orders){
        if(actO == new){
            desc = desc xor true
        }
        else{
            desc = false
            actO = new
        }
    }

    fun Resort(v: View){
        clear()
        val i = v.id

        when(i){
            R.id.IDtv->{
                checkSet(Orders._ID)
            }
            R.id.TITLEtv->{
                checkSet(Orders.TITLE)
            }
            R.id.YEARtv->{
                checkSet(Orders.YEAR_PUB)
            }
            R.id.RANKtv->{
                if(!addons){
                    checkSet(Orders.RANK_POS)
                }
            }
        }
        val dbHandler = DBHandlerMain(this, null, null,  1)
        val theList = dbHandler.getVals(addons.toInt(), actO, desc)
        makeTable(theList)
    }
    override fun finish() {
        setResult(Activity.RESULT_OK, null)
        super.finish()
    }
}