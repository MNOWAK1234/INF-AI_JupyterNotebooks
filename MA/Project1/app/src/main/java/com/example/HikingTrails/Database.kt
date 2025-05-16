package com.example.HikingTrails

import android.content.Context
import androidx.compose.runtime.saveable.Saver
import androidx.room.Dao
import androidx.room.Database
import androidx.room.Delete
import androidx.room.Entity
import androidx.room.Insert
import androidx.room.PrimaryKey
import androidx.room.Query
import androidx.room.Room
import androidx.room.RoomDatabase
import androidx.room.Update

@Entity(tableName = "trails")
data class Trail(
    @PrimaryKey val id: Int,
    val name: String,
    val distance: Double,
    val time: Int,
    val description: String,
    val level: Int
)

@Entity(tableName = "times")
data class Time(
    @PrimaryKey val id: Int,
    val trailID: Int,
    val time: Long,
    val date: String
)

@Entity(tableName = "achievements")
data class Achievement(
    @PrimaryKey val id: Int,
    val text: String,
    val achieved: Boolean = false
)

@Entity(tableName = "GlobalVariables")
data class GlobalVariable(
    @PrimaryKey val id: Int,
    val variableToRemember: Double
)

class CurrentTrail(var id: Int){
    companion object{
        val Saver: Saver<CurrentTrail?, Int> = Saver({it?.id}, ::CurrentTrail)
    }
}

class CurrentTabIndex(var id: Int){
    companion object{
        val Saver: Saver<CurrentTabIndex?, Int> = Saver({it?.id}, ::CurrentTabIndex)
    }
}

@Dao
interface TrailDAO {
    @Insert
    fun insert(trail: Trail)

    @Update
    fun update(trail: Trail)

    @Delete
    fun delete(trail: Trail)

    @Query("SELECT COUNT(*) FROM trails")
    fun getTrailCount(): Int

    @Query("SELECT * FROM trails")
    fun getAllTrails(): List<Trail>

    @Query("SELECT * FROM trails WHERE level <= (:level)")
    fun getEasyTrails(level: Int): List<Trail>

    @Query("SELECT * FROM trails WHERE level > (:level)")
    fun getHardTrails(level: Int): List<Trail>

    @Query("SELECT * FROM trails where id = (:trailID)")
    fun getTrailDescription(trailID: Int): Trail

    @Query("SELECT id FROM trails where name = (:trailName)")
    fun getIdFromName(trailName: String): Int
}

@Dao
interface TimesDAO {
    @Insert
    fun insert(time: Time)

    @Update
    fun update(time: Time)

    @Delete
    fun delete(time: Time)

    @Query("SELECT COUNT(*) FROM times")
    fun getTrailTimesCount(): Int

    @Query("SELECT * FROM times ORDER BY id ASC")
    fun getAllTimes(): List<Time>

    @Query("SELECT MAX(id)+1 FROM times")
    fun genNextIndex(): Int

    @Query("SELECT * FROM times where trailID = (:trailID)")
    fun getAllTimesForOneTrail(trailID: Int): List<Time>
}

@Dao
interface AchievementDAO {
    @Insert
    fun insert(achievement: Achievement)

    @Update
    fun update(achievement: Achievement)

    @Delete
    fun delete(achievement: Achievement)

    @Query("SELECT COUNT(*) FROM achievements")
    fun getAchievementCount(): Int

    @Query("SELECT * FROM achievements WHERE achieved = 1")
    fun getAchieved(): List<Achievement>

    @Query("SELECT * FROM achievements")
    fun getAllAchievements(): List<Achievement>

    @Query("UPDATE achievements SET achieved = 1 WHERE id = :id")
    fun completeAchievement(id: Int)
}

@Dao
interface GlobalVariablesDAO {
    @Insert
    fun insert(variables: GlobalVariable)

    @Query("UPDATE GlobalVariables SET variableToRemember = :timeModifier WHERE id = 1")
    fun updateTimeModifier(timeModifier: Double)

    @Query("SELECT variableToRemember FROM GlobalVariables WHERE id = 1")
    fun getTimeModifier(): Double

    @Query("SELECT COUNT(*) FROM GlobalVariables")
    fun getVariablesCount(): Int
}

@Database(entities = [Trail::class, Time::class, Achievement::class, GlobalVariable::class], version = 4)
abstract class AppDatabase : RoomDatabase() {
    abstract fun TrailDAO(): TrailDAO
    abstract fun TimesDAO(): TimesDAO
    abstract fun AchievementDAO(): AchievementDAO
    abstract fun GlobalVariablesDAO(): GlobalVariablesDAO

    companion object {
        @Volatile
        private var INSTANCE: AppDatabase? = null

        fun getInstance(context: Context): AppDatabase =
            INSTANCE ?: synchronized(this) {
                INSTANCE ?: buildDatabase(context).also { INSTANCE = it }
            }

        private fun buildDatabase(context: Context) =
            Room.databaseBuilder(
                context.applicationContext,
                AppDatabase::class.java, "trails.db"
            ).fallbackToDestructiveMigration().build()
    }
}