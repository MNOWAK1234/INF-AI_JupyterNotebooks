package com.example.HikingTrails

import androidx.compose.material.icons.Icons
import androidx.compose.material.icons.filled.Menu
import androidx.compose.material3.CenterAlignedTopAppBar
import androidx.compose.material3.DrawerState
import androidx.compose.material3.ExperimentalMaterial3Api
import androidx.compose.material3.Icon
import androidx.compose.material3.IconButton
import androidx.compose.material3.MaterialTheme
import androidx.compose.material3.Text
import androidx.compose.material3.TopAppBarDefaults
import androidx.compose.material3.rememberTopAppBarState
import androidx.compose.runtime.Composable
import androidx.compose.ui.platform.LocalContext
import androidx.navigation.NavHostController
import kotlinx.coroutines.CoroutineScope
import kotlinx.coroutines.launch

@OptIn(ExperimentalMaterial3Api::class)
@Composable
fun CustomAppBar(drawerState: DrawerState?, title: String, navController: NavHostController, database: AppDatabase, coroutineScope: CoroutineScope, timerViewModel: TimerViewModel, onShowPopUpChange: (Boolean)->Unit) {
    val context = LocalContext.current
    CenterAlignedTopAppBar(
        scrollBehavior = TopAppBarDefaults.enterAlwaysScrollBehavior(state = rememberTopAppBarState(
            0.0F,
            0.0F,
            0.0F
        ), canScroll = { true }),
        navigationIcon = {
            if (drawerState != null) {
                IconButton(onClick = {
                    coroutineScope.launch {
                        drawerState.open()
                    }
                }) {
                    Icon(Icons.Filled.Menu, contentDescription = "",  tint = MaterialTheme.colorScheme.tertiary)
                }
            }
        },
        title = { Text(text = title) },
        colors = TopAppBarDefaults.topAppBarColors(
            containerColor = MaterialTheme.colorScheme.primaryContainer,
            titleContentColor = MaterialTheme.colorScheme.tertiary,
        ),
    )
}