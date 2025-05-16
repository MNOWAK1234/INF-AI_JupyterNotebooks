package com.example.HikingTrails

import android.content.res.Configuration
import android.os.Bundle
import androidx.activity.ComponentActivity
import androidx.activity.compose.setContent
import androidx.activity.viewModels
import androidx.compose.foundation.layout.fillMaxSize
import androidx.compose.material3.MaterialTheme
import androidx.compose.material3.Surface
import androidx.compose.runtime.Composable
import androidx.compose.runtime.mutableStateOf
import androidx.compose.runtime.saveable.rememberSaveable
import androidx.compose.ui.Modifier
import androidx.navigation.NavController
import androidx.navigation.compose.rememberNavController
import androidx.room.Room
import com.example.HikingTrails.ui.theme.MainTheme

@Composable
fun Main(device: String, database: AppDatabase, timerViewModel: TimerViewModel, tabList: List<TabItem>, navController: NavController){
    val selectedTrail = rememberSaveable(stateSaver = CurrentTrail.Saver){
        mutableStateOf(null)
    }
    val selectedTabIndex = rememberSaveable(stateSaver = CurrentTabIndex.Saver){
        mutableStateOf(CurrentTabIndex(0))
    }
    Surface(modifier = Modifier.fillMaxSize(), color = MaterialTheme.colorScheme.background) {
        LayoutTabs(database, timerViewModel, tabList, selectedTrail, selectedTabIndex, device)
    }
}

class MainActivity : ComponentActivity() {
    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        val database = Room.databaseBuilder(applicationContext, AppDatabase::class.java, "routes.db")
            .allowMainThreadQueries()
            .fallbackToDestructiveMigration()
            .build()
        populateDatabase(database)

        val timerViewModel: TimerViewModel by viewModels()

        val tabList = listOf(
            TabItem("Help"),
            TabItem("Easy"),
            TabItem("Hard"),
            TabItem("Rewards")
        )
        setContent {
            MainTheme {
                val orientation = this.resources.configuration.orientation
                val navController = rememberNavController()
                val device = if (orientation == Configuration.ORIENTATION_LANDSCAPE)
                {
                    // The width of the screen is greater than the height
                    "Tablet"
                } else {
                    "Phone"
                }
                SetupNavGraph(navController, database, timerViewModel, tabList, device)
            }
        }
    }
}
