package com.example.HikingTrails

import androidx.compose.runtime.Composable
import androidx.navigation.NavHostController
import androidx.navigation.compose.NavHost
import androidx.navigation.compose.composable

@Composable
fun SetupNavGraph(navController: NavHostController, database: AppDatabase, timerViewModel: TimerViewModel, tabList: List<TabItem>, device: String) {
    NavHost(
        navController = navController,
        startDestination = Screens.Splash.option
    ) {
        composable(route = Screens.Splash.option) {
            AnimatedSplashScreen(navController = navController)
        }
        composable(route = Screens.Home.option) {
            Main(device, database, timerViewModel, tabList, navController)
        }
    }
}