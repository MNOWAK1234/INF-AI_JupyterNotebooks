package com.example.HikingTrails

import androidx.compose.foundation.layout.fillMaxWidth
import androidx.compose.runtime.Composable
import androidx.compose.runtime.MutableState
import androidx.compose.ui.Modifier
import androidx.navigation.NavType
import androidx.navigation.compose.NavHost
import androidx.navigation.compose.composable
import androidx.navigation.compose.rememberNavController
import androidx.navigation.navArgument

@Composable
fun PagePhoneTrails(database: AppDatabase, timerViewModel: TimerViewModel, isDifficult: Boolean, selectedRoute: MutableState<CurrentTrail?>){
    val routeList =
        if (isDifficult) database.TrailDAO().getHardTrails(5)
        else database.TrailDAO().getEasyTrails(5)
    val navController = rememberNavController()
    NavHost(navController = navController, startDestination = "LayoutPhoneDetailHorizontalLeft") {
        composable(route = "LayoutPhoneDetailHorizontalLeft"){
            LayoutPhoneMainVertical(navController = navController, trailList = routeList, database = database, onSavableTrailIndexChange = { selectedRoute.value = CurrentTrail(it) })
        }
        composable(
            route = "LayoutPhoneDetailHorizontalRight" + "/{routeID}",
            arguments = listOf(
                navArgument("routeID") {
                    type = NavType.IntType
                }
            )
        ){ entry ->
            entry.arguments?.getInt("routeID")
                ?.let { LayoutPhoneDetailVertical(index = it, modifier = Modifier.fillMaxWidth(), database = database, timerViewModel = timerViewModel) }
        }
    }
}