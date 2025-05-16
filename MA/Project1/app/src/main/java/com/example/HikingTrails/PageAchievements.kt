package com.example.HikingTrails

import androidx.compose.foundation.Image
import androidx.compose.foundation.background
import androidx.compose.foundation.layout.Arrangement
import androidx.compose.foundation.layout.Box
import androidx.compose.foundation.layout.fillMaxSize
import androidx.compose.foundation.layout.fillMaxWidth
import androidx.compose.foundation.layout.height
import androidx.compose.foundation.layout.padding
import androidx.compose.foundation.lazy.grid.GridCells
import androidx.compose.foundation.lazy.grid.LazyVerticalGrid
import androidx.compose.foundation.lazy.grid.items
import androidx.compose.foundation.shape.RoundedCornerShape
import androidx.compose.material3.Card
import androidx.compose.material3.CardDefaults
import androidx.compose.material3.Text
import androidx.compose.runtime.Composable
import androidx.compose.runtime.remember
import androidx.compose.ui.Alignment
import androidx.compose.ui.Modifier
import androidx.compose.ui.graphics.Brush
import androidx.compose.ui.graphics.Color
import androidx.compose.ui.layout.ContentScale
import androidx.compose.ui.platform.LocalConfiguration
import androidx.compose.ui.platform.LocalDensity
import androidx.compose.ui.res.painterResource
import androidx.compose.ui.text.TextStyle
import androidx.compose.ui.unit.dp
import androidx.compose.ui.unit.sp

@Composable
fun calculateNumColumns(screenWidth: Int, imageWidth: Int): Int {

    var numColumns = screenWidth / imageWidth

    if (numColumns < 1) {
        numColumns = 1
    }
    return numColumns
}

data class RewardImage(
    val imageResId: Int
)

data class AchievementPlusImage(
    val imageResId: Int,
    val id: Int,
    val text: String,
    val achieved: Boolean
)

@Composable
fun PageRewards(database: AppDatabase) {
    val rewardImages = listOf(
        RewardImage(R.drawable.easy_trail_image),
        RewardImage(R.drawable.medium_trail_image),
        RewardImage(R.drawable.hard_trail_image),
        RewardImage(R.drawable.very_hard_trail_image),
        RewardImage(R.drawable.impossible_trail_image),
        RewardImage(R.drawable.extreme_trail_image),
        RewardImage(R.drawable.easy_trail_image),
        RewardImage(R.drawable.medium_trail_image),
        RewardImage(R.drawable.hard_trail_image)
    )

    val achievements = remember { database.AchievementDAO().getAllAchievements() }

    val achievedRewards = achievements.filter { it.achieved }

    val combinedRewards: List<AchievementPlusImage> = achievedRewards.map { achievement ->
        val correspondingReward = rewardImages[achievement.id]
        AchievementPlusImage(
            imageResId = correspondingReward.imageResId,
            id = achievement.id,
            text = achievement.text,
            achieved = achievement.achieved
        )
    }

    val screenWidth = LocalConfiguration.current.screenWidthDp
    val imageWidth = with(LocalDensity.current) {
        val density = LocalDensity.current.density
        val dpWidth = LocalConfiguration.current.screenWidthDp
        val isTablet = (dpWidth / density) >= 400 // Adjust the threshold as needed

        if (isTablet) {
            200.dp.toPx().toInt() // Set image width for tablets
        } else {
            100.dp.toPx().toInt() // Set image width for smartphones
        }
    }
    val numColumns = calculateNumColumns(screenWidth, imageWidth)

    val columnSpacing = 8.dp // Adjust the column spacing as needed
    val rowSpacing = 8.dp // Adjust the row spacing as needed

    LazyVerticalGrid(
        columns = GridCells.Fixed(numColumns),
        modifier = Modifier
            .fillMaxSize()
            .padding(all = 8.dp),
        verticalArrangement = Arrangement.spacedBy(rowSpacing),
        horizontalArrangement = Arrangement.spacedBy(columnSpacing)
    ) {
        items(items = combinedRewards) { rewardWithAchievement ->
            RewardCard(achievement = rewardWithAchievement)
        }
    }
}

@Composable
fun RewardCard(
    achievement: AchievementPlusImage,
    modifier: Modifier = Modifier
) {
    Card(
        modifier = modifier.fillMaxWidth(),
        shape = RoundedCornerShape(15.dp),
        elevation = CardDefaults.cardElevation(defaultElevation = 5.dp)
    ) {
        Box(modifier = Modifier.height(200.dp)) {
            Image(
                painter = painterResource(id = achievement.imageResId),
                contentDescription = null, // Content description can be null if not applicable
                contentScale = ContentScale.FillBounds,
                modifier = Modifier.fillMaxSize()
            )
            Box(
                modifier = Modifier
                    .fillMaxSize()
                    .background(
                        Brush.verticalGradient(
                            colors = listOf(
                                Color.Transparent,
                                Color.Black
                            ),
                            startY = 300f
                        )
                    )
            )
            Box(
                modifier = Modifier
                    .fillMaxSize()
                    .padding(12.dp),
                contentAlignment = Alignment.BottomStart
            ) {
                Text(
                    achievement.text,
                    style = TextStyle(color = Color.White, fontSize = 16.sp)
                )
            }
        }
    }
}