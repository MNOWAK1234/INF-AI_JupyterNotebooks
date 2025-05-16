package com.example.HikingTrails

import SearchbarContent
import SearchbarView
import android.annotation.SuppressLint
import androidx.compose.foundation.clickable
import androidx.compose.foundation.layout.Column
import androidx.compose.foundation.layout.fillMaxSize
import androidx.compose.foundation.layout.padding
import androidx.compose.foundation.lazy.staggeredgrid.LazyVerticalStaggeredGrid
import androidx.compose.foundation.lazy.staggeredgrid.StaggeredGridCells
import androidx.compose.material3.HorizontalDivider
import androidx.compose.material3.MaterialTheme
import androidx.compose.material3.Scaffold
import androidx.compose.material3.Surface
import androidx.compose.runtime.Composable
import androidx.compose.ui.Modifier
import androidx.compose.ui.graphics.Color
import androidx.navigation.NavController

@SuppressLint("UnusedMaterial3ScaffoldPaddingParameter")
@Composable
fun LayoutPhoneMainVertical(
    modifier: Modifier = Modifier,
    navController: NavController,
    trailList: List<Trail>,
    onClick: ((CurrentTrail) -> Unit)? = null,
    database: AppDatabase,
    onSavableTrailIndexChange: (Int) -> Unit
) {
    Surface(color = Color.Black) {
        Scaffold(
            topBar = {
                SearchbarContent(
                    searchbarView = SearchbarView(trailList),
                    navController = navController,
                    showPopupChange = { false }
                )
            }
        ) { innerPadding ->
            Surface(color = Color.Black) {
                Column(modifier = Modifier.padding(innerPadding)) {
                    LazyVerticalStaggeredGrid(
                        columns = StaggeredGridCells.Fixed(2),
                        modifier = Modifier.fillMaxSize()
                    ) {
                        items(trailList.size) { index ->
                            if (onClick == null) {
                                TrailCard(
                                    trailList[index].name,
                                    trailList[index].id,
                                    modifier = modifier.clickable {
                                        val arg = trailList[index].id.toString()
                                        navController.navigate("LayoutPhoneDetailHorizontalRight/$arg")
                                        onSavableTrailIndexChange(trailList[index].id)
                                    }
                                )
                            } else {
                                TrailCard(
                                    trailList[index].name,
                                    trailList[index].id,
                                    modifier = modifier.clickable {
                                        onClick(CurrentTrail(index))
                                    }
                                )
                            }
                        }
                    }
                }
            }
        }
    }
}