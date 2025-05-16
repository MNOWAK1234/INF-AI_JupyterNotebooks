package com.example.HikingTrails

import androidx.compose.foundation.clickable
import androidx.compose.foundation.layout.consumeWindowInsets
import androidx.compose.foundation.lazy.staggeredgrid.LazyVerticalStaggeredGrid
import androidx.compose.foundation.lazy.staggeredgrid.StaggeredGridCells
import androidx.compose.material3.Scaffold
import androidx.compose.material3.Surface
import androidx.compose.runtime.Composable
import androidx.compose.ui.Modifier
import androidx.compose.ui.graphics.Color
import androidx.navigation.NavController

@Composable
fun LayoutPhoneDetailHorizontalLeft(modifier: Modifier = Modifier, navController: NavController, trailList: List<Trail>, onClick: ((CurrentTrail) -> Unit)? = null) {
    Surface(color = Color.Black) {
        Scaffold(
            modifier = modifier
        ) { innerPadding ->
            Surface(color = Color.Black) {
                LazyVerticalStaggeredGrid(
                    columns = StaggeredGridCells.Fixed(2),
                    modifier = Modifier.consumeWindowInsets(innerPadding),
                    contentPadding = innerPadding,
                ) {
                    items(trailList.size) { index ->
                        if (onClick == null) {
                            TrailCard(
                                trailList[index].name,
                                trailList[index].id,
                                modifier = Modifier.clickable {
                                    val arg = trailList[index].id.toString()
                                    navController.navigate("LayoutPhoneDetailHorizontalRight/$arg")
                                }
                            )
                        } else {
                            TrailCard(
                                trailList[index].name,
                                trailList[index].id,
                                modifier = Modifier.clickable {
                                    onClick(CurrentTrail(trailList[index].id))
                                }
                            )
                        }
                    }
                }
            }
        }
    }
}