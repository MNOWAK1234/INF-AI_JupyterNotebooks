package com.example.inf151813

import android.app.Activity
import android.content.Context
import android.content.Intent
import android.net.ConnectivityManager
import android.net.NetworkCapabilities
import android.os.AsyncTask
import android.os.Build
import androidx.appcompat.app.AppCompatActivity
import android.os.Bundle
import android.view.View
import android.widget.Button
import android.widget.EditText
import android.widget.Toast
import androidx.appcompat.app.AlertDialog
import kotlinx.coroutines.async
import kotlinx.coroutines.delay
import kotlinx.coroutines.runBlocking
import org.w3c.dom.Document
import org.w3c.dom.Element
import org.w3c.dom.Node
import org.w3c.dom.NodeList
import java.io.File
import java.io.FileNotFoundException
import java.io.FileOutputStream
import java.io.FileWriter
import java.io.IOException
import java.net.MalformedURLException
import java.net.URL
import java.nio.file.Files
import java.nio.file.Paths
import java.text.SimpleDateFormat
import java.util.ArrayList
import java.util.Date
import javax.xml.parsers.DocumentBuilderFactory
import kotlin.system.measureTimeMillis

class MainActivity : AppCompatActivity() {

    fun Boolean.toInt() = if (this) 1 else 0
    var saveFinished: Boolean = false
    var downloadFinished: Boolean = false
    var userName: String ? = "MNOWAK1234"
    var lastSync: String = "2023-06-01"
    var games: Int = 0
    var addons: Int = 0
    var globalAddonsCheck: Int = 1
    var expired: Boolean = false
    val sdf = SimpleDateFormat("yyyy-MM-dd")
    val filesDir = "/data/data/com.example.inf151813/files"
    val toCheck: MutableList<String> = ArrayList()

    lateinit var progressDialog: AlertDialog
    lateinit var userButton: Button
    lateinit var syncButton: Button
    lateinit var gamesButton: Button
    lateinit var addsButton: Button
    lateinit var changeButton: Button
    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        setContentView(R.layout.activity_main)
        userButton = findViewById(R.id.User)
        syncButton = findViewById(R.id.Sync)
        gamesButton = findViewById(R.id.Games)
        addsButton = findViewById(R.id.Adds)
        changeButton = findViewById(R.id.Change)
        userName = getFirstSubdirectoryName(filesDir)
        checkUser()
        userButton.text = "Hello $userName"
        personalize()
    }

    fun checkUser(){
        if(isInternetAvailable(this)){
            Toast.makeText(this, "Connected", Toast.LENGTH_LONG).show()
            //check if we have info about user
            if (userName != null) {
                println("I found data about user: $userName")
                val userdir = filesDir+"/"+userName
                println(userdir)
                val filedata = "$userdir/UserData.txt"
                val filegames = "$userdir/UserGames.txt"
                var userData: List<String> = File(filedata).bufferedReader().readLines()
                userName = userData[0]
                lastSync = userData[1]
                var userGames: List<String> = File(filegames).bufferedReader().readLines()
                games = Integer.parseInt(userGames[0])
                addons = Integer.parseInt(userGames[1])

            } else {
                println("There are no user data")
                println("Creating user data file")
                NewAccount()
                printDatabaseContents()
            }
        }
        else{
            val builder = AlertDialog.Builder(this@MainActivity)
            builder.setMessage("This app requires internet connection.\nTry again.")
                .setCancelable(false)
                .setPositiveButton("OK") { dialog, id ->
                    dialog.dismiss()
                    finish()
                }
            val alert = builder.create()
            alert.show()
        }
    }

    fun isInternetAvailable(context: Context): Boolean {
        // Get the ConnectivityManager service from the context
        val connectivityManager = context.getSystemService(Context.CONNECTIVITY_SERVICE) as ConnectivityManager

        // Check if the Android version is Marshmallow or higher
        if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.M) {
            // Get the active network from the ConnectivityManager
            val network = connectivityManager.activeNetwork ?: return false

            // Get the network capabilities of the active network
            val activeNetwork = connectivityManager.getNetworkCapabilities(network) ?: return false

            // Check the network transport type
            return when {
                activeNetwork.hasTransport(NetworkCapabilities.TRANSPORT_WIFI) -> true // WiFi is available
                activeNetwork.hasTransport(NetworkCapabilities.TRANSPORT_CELLULAR) -> true // Cellular data is available
                else -> false // No internet connectivity
            }
        } else {
            // For Android versions below Marshmallow, use the deprecated activeNetworkInfo property
            @Suppress("DEPRECATION")
            val networkInfo = connectivityManager.activeNetworkInfo ?: return false

            // Check if the network is connected
            @Suppress("DEPRECATION")
            return networkInfo.isConnected
        }
    }

    fun NewAccount() {
        val popupView: View = layoutInflater.inflate(R.layout.popup, null)
        val userNameField: EditText = popupView.findViewById(R.id.editUsername)
        val accept: Button = popupView.findViewById(R.id.okButton)
        val dialogBuilder = AlertDialog.Builder(this).setView(popupView)
        val dialog: AlertDialog = dialogBuilder.create()
        dialog.show()
        accept.setOnClickListener() {
            if (userNameField.text.toString() == "") {
                Toast.makeText(this, "Empty username", Toast.LENGTH_SHORT).show()
            }else{
                userName = userNameField.text.toString()
                val userdir = filesDir+"/"+userName
                createDirectoryAndFiles(userdir)
                userButton.text = "Hello $userName\nClear your data and exit"
                dialog.dismiss()
                Toast.makeText(this, "Syncing...", Toast.LENGTH_LONG).show()
                synchronize(userName)
            }
        }
    }

    fun synchronize(user: String ?) {
        downloadingAnimation()
        val currentDate = sdf.format(Date())
        lastSync = currentDate
        userName = user
        var query = "https://boardgamegeek.com/xmlapi2/collection?username=$user&stats=1&own=1&excludesubtype=boardgameexpansion"
        /*Note that starting from Android API level 30, AsyncTask is deprecated.
        It is recommended to use other concurrency constructs like Executor, Coroutine, or Thread
        for performing background tasks in newer Android projects.*/
        downloadData(query, user, true, true)
        while(!saveFinished){}
        saveFinished=false
        query = "https://boardgamegeek.com/xmlapi2/collection?username=$user&stats=1&own=1&subtype=boardgameexpansion"
        downloadData(query, user, false, false)
        while(!downloadFinished){}
        finishSync(user)

    }

    fun downloadingAnimation(){
        val progressView: View = layoutInflater.inflate(R.layout.downloading, null)
        val dialogBuilder = AlertDialog.Builder(this@MainActivity)
        dialogBuilder.setView(progressView)
        progressDialog = dialogBuilder.create()
        progressDialog.show()
    }

    fun downloadData(q: String, user: String ?, games:Boolean, main:Boolean) {
        downloadFinished = false
        val stored = DataDownloader()
        stored.setData(q, user, games, main)
        /*The execute method in the AsyncTask class is used to
        start the execution of the background task defined in the doInBackground method.
        It initiates the asynchronous execution of the task.*/
        stored.execute()
    }

    @Suppress("DEPRECATION")
    private inner class DataDownloader: AsyncTask<String, Int, String>(){

        var queryURL: String = ""
        var user: String ?= ""
        var main: Boolean = false
        var games: Boolean = false

        fun setData(adress: String, name: String ?,games: Boolean, where:Boolean){
            this.queryURL = adress
            this.user = name
            this.main = where
            this.games = games
        }

        override fun onPreExecute() {
            super.onPreExecute()
            downloadFinished = false
        }

        override fun onPostExecute(result: String?) {
            super.onPostExecute(result)
        }

        override fun onProgressUpdate(vararg values: Int?) {
            super.onProgressUpdate(*values)
        }


        override fun doInBackground(vararg p0: String?): String {

            try {
                val url = URL(queryURL) // Create a URL object
                val connection = url.openConnection() // Open a connection
                connection.connect() // Connect to the URL
                val istream = connection.getInputStream() // Get the input stream to read the content
                val content = istream.bufferedReader().use { it.readText() } // Read the content from the input stream
                val lenghtOfFile = content.length // Get the length
                val isStream = url.openStream() // Open a new input stream from the URL
                var fos : FileOutputStream
                if(games==true){
                    fos = FileOutputStream("$filesDir/$user/UserGames.xml") // Create a FileOutputStream to write the downloaded data to a file
                }
                else{
                    fos = FileOutputStream("$filesDir/$user/UserAdditions.xml") // Create a FileOutputStream to write the downloaded data to a file
                }
                val data = ByteArray(1024)
                var count = 0
                count = isStream.read(data)  // Read the first chunk of data from the input stream
                while(count != -1){
                    fos.write(data, 0, count)
                    count = isStream.read(data)
                }
                isStream.close()
                fos.close()
            }catch (e: MalformedURLException){
                return "Invalid URL"
            }catch (e: FileNotFoundException){
                return "File not found"
            }catch (e: IOException){
                return "IO Exception"
            }
            while(!save(games, main)){
                if(!expired){
                    waitawhile()
                }
                else{
                    runOnUiThread(Runnable() {
                        run() {
                            Toast.makeText(this@MainActivity, "Error", Toast.LENGTH_LONG)
                        }
                    })
                    waitawhile()
                    finish()
                }
            }
            downloadFinished = true
            return "finished"
        }
    }

    fun getFirstSubdirectoryName(directoryPath: String?): String? {
        val directory = File(directoryPath)

        if (directory.exists() && directory.isDirectory) {
            val files = directory.listFiles()

            if (files != null && files.isNotEmpty()) {
                for (file in files) {
                    if (file.isDirectory) {
                        return file.name
                    }
                }
            }
        }

        return null
    }

    fun createDirectoryAndFiles(directoryPath: String) {

        val UserDirectoryPath = directoryPath

        // Create the directory
        val directory = File(UserDirectoryPath)
        if (!directory.exists()) {
            val isDirectoryCreated = directory.mkdirs()
            if (isDirectoryCreated) {
                println("Directory created: $UserDirectoryPath")
            } else {
                println("Failed to create directory: $UserDirectoryPath")
                return
            }
        } else {
            println("Directory already exists: $UserDirectoryPath")
        }

        // Create the files inside the directory
        val file1 = File(directory, "UserData.xml")
        val file2 = File(directory, "UserGames.xml")
        val file3 = File(directory, "UserAdditions.xml")

        try {
            file1.createNewFile()
            file2.createNewFile()
            file3.createNewFile()
            println("Files created successfully.")
        } catch (e: Exception) {
            println("Failed to create files.")
            e.printStackTrace()
        }
    }

    fun save(games: Boolean, main: Boolean): Boolean {
        val dir = File("$filesDir/$userName") // Create a File object representing the directory
        var filename : String
        if(games == true){
            filename = "$dir/UserGames.xml" // Create the file path/name
        }
        else{
            filename = "$dir/UserAdditions.xml" // Create the file path/name
        }
        val file = File(filename) // Create a File object representing the file
        var xmlDoc: Document
        var valid: Boolean = false // Variable to keep track of the validity of the XML file
        var failed = 0 // Counter variable

        while (!valid) {
            if (failed == 15) {
                expired = true // Set the expired flag to true if the file access fails multiple times
                return false // Return false to indicate failure
            }
            valid = true
            try {
                xmlDoc = DocumentBuilderFactory.newInstance().newDocumentBuilder().parse(file) // Parse the XML file
            } catch (e: Exception) {
                e.printStackTrace()
                valid = false // Set the validity flag to false if an exception occurs
                failed++
            }
        }

        xmlDoc = DocumentBuilderFactory.newInstance().newDocumentBuilder().parse(file) // Parse the XML file again
        xmlDoc.documentElement.normalize()
        val items: NodeList = xmlDoc.getElementsByTagName("item") // Get a NodeList of "item" elements in the XML
        /*for (i in 0 until items.length) {
            val itemNode: Node = items.item(i)
            if (itemNode.nodeType == Node.ELEMENT_NODE) {
                val element = itemNode as Element
                // Access specific data within the "item" element and print it
                val itemText = element.textContent
                println("Item $i: $itemText")
            }
        }*/

        saveDataMain(items, !main)
        return true
    }

    suspend fun sleep(): Int {
        delay(4000L) // pretend we are doing something useful here
        return 1
    }

    fun waitawhile() = runBlocking<Unit> {
        val time = measureTimeMillis {
            val one = async { sleep() }
            one.await()
        }
    }

    fun saveDataMain(items: NodeList, small: Boolean) {
        val dbHandler = DBHandlerMain(this, null, null,  1)
        val dbStat = DBHandlerStat(this, null, null, 1)
        if(!small)
        {dbHandler.clear()}
        for(i in 0..items.length-1){
            val itemNode: Node = items.item(i)
            if(itemNode.nodeType == Node.ELEMENT_NODE) {
                val elem = itemNode as Element
                val children = elem.childNodes
                var id: String? = null
                var title: String? = null
                var org_title: String? = null
                var year_pub: String? = null
                var rank_pos: String? = null
                var pic: String? = null
                var tmp: String? = null
                var expansion: Int = 0
                val tags = itemNode.attributes
                for(j in 0..tags.length-1){
                    val node = tags.item(j)
                    when (node.nodeName){
                        "objectid" -> {id = node.nodeValue}
                    }
                }
                for(j in 0..children.length-1) {
                    val node = children.item(j)
                    if (node is Element) {
                        when (node.nodeName) {
                            "name" -> {
                                title = node.textContent
                            }
                            "yearpublished" -> {
                                year_pub = node.textContent
                            }
                            "thumbnail" -> {
                                pic = node.textContent
                            }
                            "stats" -> {
                                val n = node.childNodes
                                for (j1 in 0..n.length - 1) {
                                    val node = n.item(j1)
                                    if (node is Element) {
                                        when (node.nodeName) {
                                            "rating" -> {
                                                val n = node.childNodes
                                                for (j2 in 0..n.length - 1) {
                                                    val node = n.item(j2)
                                                    if (node is Element) {
                                                        when (node.nodeName) {
                                                            "ranks" -> {
                                                                val n = node.childNodes
                                                                for (j3 in 0..n.length - 1) {
                                                                    val node = n.item(j3)
                                                                    if (node is Element) {
                                                                        val tags = node.attributes
                                                                        for (j4 in 0..tags.length - 1) {
                                                                            val node = tags.item(j4)
                                                                            when (node.nodeName) {
                                                                                "id" -> {
                                                                                    tmp =
                                                                                        node.nodeValue
                                                                                }
                                                                                "value" -> {
                                                                                    rank_pos =
                                                                                        node.nodeValue
                                                                                }
                                                                            }
                                                                            if (tmp == "1" && rank_pos != null) {
                                                                                break
                                                                            }
                                                                        }
                                                                    }
                                                                }
                                                            }
                                                        }
                                                    }
                                                }
                                            }
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
                var _expansion: Int = 0
                if(small){
                    _expansion = 1
                }
                if(rank_pos == "Not Ranked" || rank_pos == null){
                    rank_pos = "0"
                }
                var _id: Int = Integer.parseInt(id)
                var _title: String? = title
                var _org_title: String? = title
                var _year_pub: Int = 0
                if(year_pub == null){
                    _year_pub = 0
                }
                else{
                    _year_pub = Integer.parseInt(year_pub)
                }
                var _rank_pos: Int = Integer.parseInt(rank_pos)
                var _pic: String? = pic

                val product = Record(_id, _title, _org_title, _year_pub,_rank_pos, _pic, _expansion)
                dbHandler.addRecord(product)
                if(_rank_pos!= 0){

                    val s = Stat(_id, _rank_pos, lastSync)
                    dbStat.addStat(s)
                }
            }
        }
        saveFinished = true
    }

    fun printDatabaseContents() {
        val dbHandler = DBHandlerMain(this, null, null, 1)
        val records = dbHandler.getAllRecords()
        println(records)

        for (record in records) {
            println("aaa")
            println(record)
        }
    }

    fun clearData(v: View){
        val builder = AlertDialog.Builder(this@MainActivity)
        builder.setMessage("Are you sure you want to clear your data?")
            .setCancelable(false)
            .setPositiveButton("Yes") { dialog, id ->
                var path = Paths.get("$filesDir/$userName/UserGames.txt")
                try {
                    val result = Files.deleteIfExists(path)
                    if (result) {
                        println("Deletion succeeded.")
                    } else {
                        println("Deletion failed.")
                    }
                } catch (e: IOException) {
                    println("Deletion failed.")
                    e.printStackTrace()
                }
                val dbHandler = DBHandlerMain(this, null, null,  1)
                dbHandler.clear()
                val dbStat = DBHandlerStat(this, null,null, 1)
                dbStat.clear()
                finish()
            }
            .setNegativeButton("No") { dialog, id ->
                dialog.dismiss()
            }
        val alert = builder.create()
        alert.show()
    }

    fun ChangeAccount(){
        NewAccount()
    }

    fun finishSync(user: String ?) {

        val dbHandler = DBHandlerMain(this, null, null, 1)
        games = dbHandler.countGames()
        addons = dbHandler.countAddons()
        val currentDate = sdf.format(Date())
        lastSync = currentDate
        personalize()
        //write userdata
        var filename = "$filesDir/$user/UserData.txt"
        var file = File(filename)
        if (!file.exists()) {
            file.createNewFile()
        }
        file.bufferedWriter().use { out ->
            out.write("$userName\n")
            out.write("$lastSync\n")
        }
        //write usergames
        filename = "$filesDir/$user/UserGames.txt"
        file = File(filename)
        if (!file.exists()) {
            file.createNewFile()
        }
        file.bufferedWriter().use { out ->
            out.write("$games\n")
            out.write("$addons\n")
        }
        run { Toast.makeText(this, "Finished", Toast.LENGTH_LONG) }
        progressDialog.dismiss()
    }

    fun daysFromLastSync(one: String, two: String): Long {
        var spt = one.split('-')
        val oneyear = (spt[0].toLong())
        val onemonth = (spt[1].toLong())
        val oneday = (spt[2].toLong())
        spt = two.split('-')
        val twoyear = (spt[0].toLong())
        val twomonth = (spt[1].toLong())
        val twoday = (spt[2].toLong())
        val year_diff = twoyear - oneyear
        val month_diff = twomonth - onemonth
        val day_diff = twoday - oneday
        val diff = year_diff * 365 + month_diff * 30 + day_diff
        return diff
    }

    fun syncClicked(v:View){
        Toast.makeText(this, "Syncing...", Toast.LENGTH_LONG).show()
        val today = sdf.format(Date())
        if(daysFromLastSync(lastSync, today) == 0L){
            val builder = AlertDialog.Builder(this@MainActivity)
            builder.setMessage("BGG hasn't synchronize their data yet.\nAre you sure you want to synchronize?")
                .setCancelable(false)
                .setPositiveButton("Yes") { dialog, id ->
                    dialog.dismiss()
                    synchronize(userName)
                }
                .setNegativeButton("No") { dialog, id ->
                    dialog.dismiss()
                }
            val alert = builder.create()
            alert.show()
        }
        else{
            synchronize(userName)
        }

    }

    override fun onActivityResult(requestCode: Int, resultCode: Int, data: Intent?) {
        super.onActivityResult(requestCode, resultCode, data)
        if(requestCode == 0 && resultCode == Activity.RESULT_OK){
            progressDialog.dismiss()
        }
    }

    fun gamesClicked(v:View){
        val intent = Intent(this, GameView::class.java)
        intent.putExtra("Additions", false)
        startActivity(intent)
    }

    fun addonsClicked(v:View){
        val intent = Intent(this, GameView::class.java)
        intent.putExtra("Additions", true)
        startActivity(intent)
    }

    fun preparePopup(){
        val progressView: View = layoutInflater.inflate(R.layout.downloading, null)
        val dialogBuilder = AlertDialog.Builder(this@MainActivity)
        dialogBuilder.setView(progressView)
        progressDialog = dialogBuilder.create()
        progressDialog.show()
    }

    fun personalize(){
        syncButton.text = "Last synchronized:\n $lastSync"
        gamesButton.text = "Games owned:\n $games"
        addsButton.text = "Add-ons owned:\n $addons"
    }

}