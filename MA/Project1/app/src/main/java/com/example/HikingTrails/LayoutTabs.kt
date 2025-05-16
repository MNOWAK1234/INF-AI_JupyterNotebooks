package com.example.HikingTrails

import androidx.compose.foundation.ExperimentalFoundationApi
import androidx.compose.foundation.layout.Arrangement
import androidx.compose.foundation.layout.Column
import androidx.compose.foundation.layout.fillMaxWidth
import androidx.compose.foundation.layout.padding
import androidx.compose.foundation.pager.HorizontalPager
import androidx.compose.foundation.pager.rememberPagerState
import androidx.compose.material3.DrawerState
import androidx.compose.material3.DrawerValue
import androidx.compose.material3.ExperimentalMaterial3Api
import androidx.compose.material3.ModalDrawerSheet
import androidx.compose.material3.ModalNavigationDrawer
import androidx.compose.material3.Scaffold
import androidx.compose.material3.Tab
import androidx.compose.material3.TabRow
import androidx.compose.material3.Text
import androidx.compose.material3.rememberDrawerState
import androidx.compose.runtime.Composable
import androidx.compose.runtime.LaunchedEffect
import androidx.compose.runtime.getValue
import androidx.compose.runtime.mutableIntStateOf
import androidx.compose.runtime.remember
import androidx.compose.runtime.rememberCoroutineScope
import androidx.compose.runtime.setValue
import androidx.compose.ui.Modifier
import androidx.compose.ui.unit.dp
import androidx.navigation.NavHostController
import androidx.navigation.compose.NavHost
import androidx.navigation.compose.composable
import androidx.navigation.compose.rememberNavController
import kotlinx.coroutines.CoroutineScope
import kotlinx.coroutines.launch
import menus
import DrawerContent
import PageMain
//import NavigationDrawer
import android.util.Log
import androidx.compose.foundation.background
import androidx.compose.material3.adaptive.ExperimentalMaterial3AdaptiveApi
import androidx.compose.material3.adaptive.rememberListDetailPaneScaffoldNavigator
import androidx.compose.runtime.MutableState
import androidx.compose.runtime.mutableStateOf
import androidx.compose.runtime.saveable.rememberSaveable
import androidx.compose.ui.graphics.Color
import androidx.compose.ui.platform.LocalConfiguration
import androidx.navigation.NavType
import androidx.navigation.navArgument

data class TabItem(
    val title: String
)

@OptIn(ExperimentalFoundationApi::class, ExperimentalMaterial3Api::class,
    ExperimentalMaterial3AdaptiveApi::class
)
@Composable
fun LayoutTabs(database: AppDatabase, timerViewModel: TimerViewModel, items: List<TabItem>, selectedRoute: MutableState<CurrentTrail?>, currentTabIndex: MutableState<CurrentTabIndex?>, device: String){
    var showPopup by rememberSaveable{
        mutableStateOf(false)
    }
    val pagerState = rememberPagerState {
        items.size
    }
    var selectedTabIndex by remember{
        mutableIntStateOf(0)
    }

    val drawerState: DrawerState = rememberDrawerState(initialValue = DrawerValue.Closed)
    val coroutineScope: CoroutineScope = rememberCoroutineScope()
    val navController: NavHostController = rememberNavController()
    var changeFlag: Boolean = false
    var configuration = LocalConfiguration.current
    var screenWidth = configuration.screenWidthDp
    var isTablet = screenWidth > 400

    val navSearchbarController = rememberNavController()
    NavHost(navController = navSearchbarController, startDestination = "BigDetailLayout/0") {
        composable(
            route = "BigDetailLayout" + "/{routeID}",
            arguments = listOf(
                navArgument("routeID") {
                    type = NavType.IntType
                }
            )
        ){ entry ->
            entry.arguments?.getInt("routeID")
                ?.let {
                    LayoutPhoneDetailVertical(index = it, modifier = Modifier.fillMaxWidth(), database = database, timerViewModel = timerViewModel) }
        }
    }

    LaunchedEffect(currentTabIndex.value?.id) {
        currentTabIndex.value?.id?.let { pagerState.animateScrollToPage(it) }
    }
    LaunchedEffect(pagerState, pagerState.isScrollInProgress) {
        if(!pagerState.isScrollInProgress){
            selectedTabIndex = pagerState.currentPage
            currentTabIndex.value?.id = pagerState.currentPage
        }
    }

    LaunchedEffect(selectedTabIndex) {
        pagerState.animateScrollToPage(selectedTabIndex)
    }

    val navigator = rememberListDetailPaneScaffoldNavigator()
    ModalNavigationDrawer(
        drawerState = drawerState,
        drawerContent = {
            ModalDrawerSheet {
                DrawerContent(menus) { drawerMenu ->
                    coroutineScope.launch {
                        drawerState.close()
                        changeFlag = true
                    }
                    navController.navigate(drawerMenu.option)
                    currentTabIndex.value?.id = drawerMenu.id
                    selectedTabIndex = drawerMenu.id
                }
            }
        },
    ) {
        NavHost(navController = navController, startDestination = MainRoute.About.name) {
            composable(MainRoute.About.name) {
                if (changeFlag) {
                    currentTabIndex.value?.id = 0
                    changeFlag = false
                }
            }
            composable(MainRoute.EasyTrails.name) {
                if (changeFlag) {
                    currentTabIndex.value?.id = 1
                    changeFlag = false
                }
            }
            composable(MainRoute.HardTrails.name) {
                if (changeFlag) {
                    currentTabIndex.value?.id = 2
                    changeFlag = false
                }
            }
            composable(MainRoute.Rewards.name) {
                if (changeFlag) {
                    currentTabIndex.value?.id = 3
                    changeFlag = false
                }
            }
        }
        Scaffold(
            topBar = {
                CustomAppBar(
                    drawerState = drawerState,
                    title = "Hiking Trails",
                    navController = navController,
                    database = database,
                    coroutineScope = coroutineScope,
                    timerViewModel = timerViewModel,
                    onShowPopUpChange = { showPopup = it }
                )
            }
        ) { innerPadding ->
            Column(
                modifier = Modifier
                    .padding(innerPadding)
                    .background(Color.Black), // Set the background color to black
                verticalArrangement = Arrangement.spacedBy(4.dp)
            ) {
                selectedTabIndex?.let {
                    TabRow(selectedTabIndex = it) {
                        items.forEachIndexed { index, item ->
                            Tab(
                                selected = index == it,
                                onClick = {
                                    currentTabIndex.value?.id = index
                                    selectedTabIndex = index
                                },
                                text = {
                                    Text(text = item.title)
                                }
                            )
                        }
                    }
                }
                HorizontalPager(
                    state = pagerState,
                    modifier = Modifier
                        .fillMaxWidth()
                        .weight(1f)
                        .background(color = Color.Black)
                ) { index ->
                    if (device == "Phone") {
                        when (index) {
                            0 -> PageMain(database)
                            1 -> PagePhoneTrails(
                                database = database,
                                timerViewModel = timerViewModel,
                                isDifficult = false,
                                selectedRoute = selectedRoute
                            )

                            2 -> PagePhoneTrails(
                                database = database,
                                timerViewModel = timerViewModel,
                                isDifficult = true,
                                selectedRoute = selectedRoute
                            )

                            3 -> PageRewards(database)
                        }
                    } else { //device == Tablet
                        when (index) {
                            0 -> PageMain(database)
                            1 -> PageTabletTrails(
                                navigator = navigator,
                                selectedRoute = selectedRoute,
                                database = database,
                                navController = navController,
                                timerViewModel = timerViewModel,
                                isDifficult = true
                            )

                            2 -> PageTabletTrails(
                                navigator = navigator,
                                selectedRoute = selectedRoute,
                                database = database,
                                navController = navController,
                                timerViewModel = timerViewModel,
                                isDifficult = false
                            )

                            3 -> PageRewards(database)
                        }
                    }
                }
            }
        }
    }
}
