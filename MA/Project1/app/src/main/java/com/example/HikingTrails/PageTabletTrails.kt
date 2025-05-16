package com.example.HikingTrails

import androidx.compose.material3.adaptive.AnimatedPane
import androidx.compose.material3.adaptive.ExperimentalMaterial3AdaptiveApi
import androidx.compose.material3.adaptive.ListDetailPaneScaffold
import androidx.compose.material3.adaptive.ListDetailPaneScaffoldRole
import androidx.compose.material3.adaptive.ThreePaneScaffoldNavigator
import androidx.compose.runtime.Composable
import androidx.compose.runtime.MutableState
import androidx.compose.ui.Modifier
import androidx.navigation.NavController

@OptIn(ExperimentalMaterial3AdaptiveApi::class)
@Composable
fun PageTabletTrails(
    navigator: ThreePaneScaffoldNavigator,
    selectedRoute: MutableState<CurrentTrail?>,
    database: AppDatabase,
    navController: NavController,
    timerViewModel: TimerViewModel,
    isDifficult: Boolean){
    val routeList =
        if (!isDifficult) database.TrailDAO().getHardTrails(5)
        else database.TrailDAO().getEasyTrails(5)
    ListDetailPaneScaffold(
        scaffoldState = navigator.scaffoldState,
        listPane = {
            AnimatedPane(modifier = Modifier) {
                LayoutPhoneDetailHorizontalLeft(onClick = {
                    selectedRoute.value = it
                    navigator.navigateTo(ListDetailPaneScaffoldRole.Detail)
                },
                    navController = navController, trailList = routeList
                )
            }
        },
        detailPane = {
            AnimatedPane(modifier = Modifier) {
                selectedRoute.value?.let { route ->
                    LayoutPhoneDetailHorizontalRight(index = route.id, modifier = Modifier, database = database, timerViewModel = timerViewModel)
                }
            }
        }
    )
}