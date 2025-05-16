package com.example.inf151813

import android.content.ContentValues
import android.content.Context
import android.database.sqlite.SQLiteDatabase
import android.database.sqlite.SQLiteOpenHelper
import androidx.core.database.getStringOrNull
import java.text.SimpleDateFormat

class Record{
    var id: Int = 0
    var title: String? = null
    var org_title: String? = null
    var year_pub: Int = 0
    var rank_pos: Int = 0
    var pic: String? = null
    var expansion: Int = 0

    constructor(
        id: Int,
        title: String?,
        org_title: String?,
        year_pub: Int,
        rank_pos: Int,
        pic: String?,
        exp: Int
    ) {
        this.id = id
        this.title = title
        this.org_title = org_title
        this.year_pub = year_pub
        this.rank_pos = rank_pos
        this.pic = pic
        this.expansion = exp
    }

    constructor(title: String?, org_title: String?, year_pub: Int, rank_pos: Int, pic: String?, exp: Int) {
        this.title = title
        this.org_title = org_title
        this.year_pub = year_pub
        this.rank_pos = rank_pos
        this.pic = pic
        this.expansion = exp
    }
}

enum class Orders{
    _ID, TITLE, YEAR_PUB, RANK_POS
}

class DBHandlerMain(context: Context, name: String?, factory: SQLiteDatabase.CursorFactory?, version: Int): SQLiteOpenHelper(context, DATABASE_NAME, factory, DATABASE_VERSION) {
    companion object {
        private val DATABASE_VERSION = 1
        private val DATABASE_NAME = "games.db"
        val TABLE_RECORDS = "games"
        val COLUMN_ID = "_id"
        val COLUMN_TITLE = "title"
        val COLUMN_ORIGINAL_TITLE = "original_title"
        val COLUMN_YEAR_PUB = "year_pub"
        val COLUMN_RANK_POS = "rank_pos"
        val COLUMN_THUMBNAIL = "pic"
        val COLUMN_EXPANSION = "expansion"
    }

    override fun onCreate(db: SQLiteDatabase) {
        val CREATE_GAMES_TABLE = ("CREATE TABLE " +
                TABLE_RECORDS + "(" +
                COLUMN_ID + " INTEGER PRIMARY KEY," +
                COLUMN_TITLE + " TEXT," +
                COLUMN_ORIGINAL_TITLE + " TEXT," +
                COLUMN_YEAR_PUB + " INTEGER," +
                COLUMN_RANK_POS + " INTEGER," +
                COLUMN_THUMBNAIL + " TEXT, "+
                COLUMN_EXPANSION + " INTEGER"+
                ")")
        db.execSQL(CREATE_GAMES_TABLE)

    }

    override fun onUpgrade(db: SQLiteDatabase, oldVersion: Int, newVersion: Int) {
        db.execSQL("DROP TABLE IF EXISTS " + TABLE_RECORDS)
        onCreate(db)
    }
    fun clear(){
        val db = this.writableDatabase
        db.execSQL("DROP TABLE IF EXISTS " + TABLE_RECORDS)
        val CREATE_GAMES_TABLE = ("CREATE TABLE " +
                TABLE_RECORDS + "(" +
                COLUMN_ID + " INTEGER PRIMARY KEY," +
                COLUMN_TITLE + " TEXT," +
                COLUMN_ORIGINAL_TITLE + " TEXT," +
                COLUMN_YEAR_PUB + " INTEGER," +
                COLUMN_RANK_POS + " INTEGER," +
                COLUMN_THUMBNAIL + " TEXT, "+
                COLUMN_EXPANSION + " INTEGER"+
                ")")
        db.execSQL(CREATE_GAMES_TABLE)

    }

    fun addRecord(record: Record) {
        val values = ContentValues()
        values.put(COLUMN_ID, record.id)
        values.put(COLUMN_TITLE, record.title)
        values.put(COLUMN_ORIGINAL_TITLE, record.org_title)
        values.put(COLUMN_YEAR_PUB, record.year_pub)
        values.put(COLUMN_RANK_POS, record.rank_pos)
        values.put(COLUMN_THUMBNAIL, record.pic)
        values.put(COLUMN_EXPANSION, record.expansion)
        val db = this.writableDatabase
        db.insert(TABLE_RECORDS, null, values)
        db.close()
    }

    fun findRecord(title: String?): Record? {
        val query =
            "SELECT * FROM $TABLE_RECORDS WHERE $COLUMN_TITLE LIKE \"$title\""
        val db = this.writableDatabase
        val cursor = db.rawQuery(query, null)
        var rec: Record? = null

        if (cursor.moveToFirst()) {
            val id = Integer.parseInt(cursor.getString(0))
            val title = cursor.getString(1)
            val org_title = cursor.getString(2)
            val year_pub = cursor.getInt(3)
            val rank_pos = cursor.getInt(4)
            val thumb = cursor.getStringOrNull(5)
            val exp = cursor.getInt(6)

            rec = Record(id, title, org_title, year_pub, rank_pos, thumb, exp)
            cursor.close()
        }

        db.close()
        return rec
    }
    fun findRecord(id: Int): Record? {
        val query =
            "SELECT * FROM $TABLE_RECORDS WHERE $COLUMN_ID = $id"
        val db = this.writableDatabase
        val cursor = db.rawQuery(query, null)
        var rec: Record? = null

        if (cursor.moveToFirst()) {
            val id = Integer.parseInt(cursor.getString(0))
            val title = cursor.getString(1)
            val org_title = cursor.getString(2)
            val year_pub = cursor.getInt(3)
            val rank_pos = cursor.getInt(4)
            val thumb = cursor.getStringOrNull(5)
            val exp = cursor.getInt(6)

            rec = Record(id, title, org_title, year_pub, rank_pos, thumb, exp)
            cursor.close()
        }

        db.close()
        return rec
    }
    fun getAllRecords(): List<String> {
        val titleList = mutableListOf<String>()
        val db = this.readableDatabase
        val selectQuery = "SELECT * FROM $TABLE_RECORDS"

        val cursor = db.rawQuery(selectQuery, null)

        // Retrieve the title column value for each row
        while (cursor.moveToNext()) {
            val title = cursor.getString(cursor.getColumnIndex(COLUMN_TITLE))
            titleList.add(title)
        }

        cursor.close()
        db.close()

        return titleList
    }

    fun countGames():Int{
        var ans = 0
        val query =
            "SELECT COUNT(*) FROM $TABLE_RECORDS WHERE $COLUMN_EXPANSION = 0"
        val db = this.writableDatabase
        val cursor = db.rawQuery(query, null)
        if (cursor.moveToFirst()) {
            ans = Integer.parseInt(cursor.getString(0))
            cursor.close()
        }
        db.close()
        return ans
    }
    fun countAddons():Int{
        var ans = 0
        val query =
            "SELECT COUNT(*) FROM $TABLE_RECORDS WHERE $COLUMN_EXPANSION = 1"
        val db = this.writableDatabase
        val cursor = db.rawQuery(query, null)
        if (cursor.moveToFirst()) {
            ans = Integer.parseInt(cursor.getString(0))
            cursor.close()
        }
        db.close()
        return ans
    }
    fun getVals(addOns: Int, ord: Orders = Orders._ID, desc: Boolean = false):List<Record>{
        val mList: MutableList<Record> = ArrayList()
        val d = if(desc) "DESC" else ""
        val o = ord.toString()
        val query =
            "SELECT * FROM $TABLE_RECORDS WHERE $COLUMN_EXPANSION = $addOns ORDER BY $o $d"
        val db = this.writableDatabase
        val cursor = db.rawQuery(query, null)
        if(cursor.count == 0){
            return mList.toList()
        }
        cursor.moveToFirst()
        do {
            var rec: Record? = null
            val id = Integer.parseInt(cursor.getString(0))
            val title = cursor.getString(1)
            val org_title = cursor.getString(2)
            val year_pub = cursor.getInt(3)
            val rank_pos = cursor.getInt(4)
            val thumb = cursor.getStringOrNull(5)
            rec = Record(id, title, org_title, year_pub, rank_pos, thumb, addOns)
            mList.add(rec)
        }while(cursor.moveToNext())
        cursor.close()
        db.close()
        val res: List<Record> = mList
        return res
    }

    fun updateAddOn(id: String, exp: Int) {
        val rec = this.findRecord(Integer.parseInt(id))!!
        val values = ContentValues()
        values.put(COLUMN_ID, rec.id)
        values.put(COLUMN_TITLE, rec.title)
        values.put(COLUMN_ORIGINAL_TITLE, rec.org_title)
        values.put(COLUMN_YEAR_PUB, rec.year_pub)
        values.put(COLUMN_RANK_POS, rec.rank_pos)
        values.put(COLUMN_THUMBNAIL, rec.pic)
        values.put(COLUMN_EXPANSION, exp)
        val db = this.writableDatabase
        db.update(TABLE_RECORDS, values, "$COLUMN_ID=?", arrayOf(id))
        db.close()
    }

}
class Stat{
    var id_of_record: Int = 0
    var position: Int = 0
    var date_of_sync: String? = null

    constructor(id_of_record: Int, position: Int, date_of_sync: String?) {
        this.id_of_record = id_of_record
        this.position = position
        this.date_of_sync = date_of_sync
    }
}
class DBHandlerStat(context: Context, name: String?, factory: SQLiteDatabase.CursorFactory?, version: Int): SQLiteOpenHelper(context, DATABASE_NAME, factory, DATABASE_VERSION) {
    companion object {
        private val DATABASE_VERSION = 1
        private val DATABASE_NAME = "stat.db"
        val TABLE_STAT = "stat"
        val COLUMN_ID = "_id"
        val COLUMN_DATE = "sync_date"
        val COLUMN_RANK_POS = "rank_pos"
    }

    override fun onCreate(db: SQLiteDatabase) {
        crt(db)
    }
    fun crt(db: SQLiteDatabase){
        val CREATE_STAT_TABLE = ("CREATE TABLE " +
                TABLE_STAT + "(" +
                COLUMN_ID + " INTEGER," +
                COLUMN_DATE + " DATE," +
                COLUMN_RANK_POS + " INTEGER, " +
                "PRIMARY KEY ("+ COLUMN_ID + ", " + COLUMN_DATE +
                ") )")
        db.execSQL(CREATE_STAT_TABLE)
    }

    override fun onUpgrade(db: SQLiteDatabase, oldVersion: Int, newVersion: Int) {
        db.execSQL("DROP TABLE IF EXISTS " + TABLE_STAT)
        onCreate(db)
    }
    fun clear(){
        val db = this.writableDatabase
        db.execSQL("DROP TABLE IF EXISTS " + TABLE_STAT)
        crt(db)
    }

    fun addStat(s: Stat) {
        if(!exist(s)){
            val values = ContentValues()
            values.put(COLUMN_ID, s.id_of_record)
            values.put(COLUMN_DATE, s.date_of_sync)
            values.put(COLUMN_RANK_POS, s.position)
            val db = this.writableDatabase
            db.insert(TABLE_STAT, null, values)
            db.close()
        }
    }
    fun exist(s: Stat):Boolean{
        val id = s.id_of_record
        val date = s.date_of_sync
        val query =
            "SELECT * FROM $TABLE_STAT WHERE $COLUMN_ID LIKE \"$id\" AND $COLUMN_DATE LIKE \"$date\""
        val db = this.writableDatabase
        val cursor = db.rawQuery(query, null)
        val ans:Boolean = (cursor.count != 0)
        cursor.close()
        db.close()
        return ans
    }
    fun getStats(id: Int): List<Stat>{
        val sdf = SimpleDateFormat("yyyy-MM-dd")
        val mList: MutableList<Stat> = ArrayList()
        val query =
            "SELECT * FROM $TABLE_STAT WHERE $COLUMN_ID = $id ORDER BY $COLUMN_DATE DESC"
        val db = this.writableDatabase
        val cursor = db.rawQuery(query, null)
        if(cursor.count == 0){
            return mList.toList()
        }
        cursor.moveToFirst()
        do {
            var s: Stat? = null
            val id = Integer.parseInt(cursor.getString(0))
            val rank_pos = cursor.getInt(2)
            val date = cursor.getString(1)
            s = Stat(id, rank_pos, date)
            mList.add(s)
        }while(cursor.moveToNext())
        cursor.close()
        db.close()
        val res: List<Stat> = mList
        return res
    }
}