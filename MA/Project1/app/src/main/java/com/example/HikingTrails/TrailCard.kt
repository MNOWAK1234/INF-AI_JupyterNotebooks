package com.example.HikingTrails

import androidx.compose.foundation.BorderStroke
import androidx.compose.foundation.Image
import androidx.compose.foundation.background
import androidx.compose.foundation.layout.*
import androidx.compose.foundation.shape.RoundedCornerShape
import androidx.compose.material3.*
import androidx.compose.runtime.Composable
import androidx.compose.ui.Alignment
import androidx.compose.ui.Modifier
import androidx.compose.ui.graphics.Brush
import androidx.compose.ui.graphics.Color
import androidx.compose.ui.layout.ContentScale
import androidx.compose.ui.platform.LocalConfiguration
import androidx.compose.ui.res.painterResource
import androidx.compose.ui.text.TextStyle
import androidx.compose.ui.text.style.TextAlign
import androidx.compose.ui.unit.dp
import androidx.compose.ui.unit.sp

data class TrailImage(
    val id: Int,
    val name: String,
    val imageResId: Int
)

val trailImageList = listOf(
    TrailImage(0, "Extreme Trail", R.drawable.extreme_trail_image),
    TrailImage(1, "Easy Trail", R.drawable.easy_trail_image),
    TrailImage(2, "Medium Trail", R.drawable.medium_trail_image),
    TrailImage(3, "Hard Trail", R.drawable.hard_trail_image),
    TrailImage(4, "Very Hard Trail", R.drawable.very_hard_trail_image),
    TrailImage(5, "Impossible Trail", R.drawable.impossible_trail_image),
    TrailImage(6, "Extreme Trail", R.drawable.extreme_trail_image),
    TrailImage(7, "Easy Trail", R.drawable.easy_trail_image),
    TrailImage(8, "Medium Trail", R.drawable.medium_trail_image),
    TrailImage(9, "Hard Trail", R.drawable.hard_trail_image),
    TrailImage(10, "Very Hard Trail", R.drawable.very_hard_trail_image),
    TrailImage(11, "Impossible Trail", R.drawable.impossible_trail_image),
    TrailImage(12, "Extreme Trail", R.drawable.extreme_trail_image),
    TrailImage(13, "Easy Trail", R.drawable.easy_trail_image),
    TrailImage(14, "Medium Trail", R.drawable.medium_trail_image),
    TrailImage(15, "Hard Trail", R.drawable.hard_trail_image),
    TrailImage(16, "Very Hard Trail", R.drawable.very_hard_trail_image),
    TrailImage(17, "Impossible Trail", R.drawable.impossible_trail_image),
    TrailImage(18, "Extreme Trail", R.drawable.extreme_trail_image),
    TrailImage(19, "Easy Trail", R.drawable.easy_trail_image),
    TrailImage(20, "Medium Trail", R.drawable.medium_trail_image),
    TrailImage(21, "Hard Trail", R.drawable.hard_trail_image),
    TrailImage(22, "Very Hard Trail", R.drawable.very_hard_trail_image),
    TrailImage(23, "Impossible Trail", R.drawable.impossible_trail_image),
    TrailImage(24, "Extreme Trail", R.drawable.extreme_trail_image),
    TrailImage(25, "Easy Trail", R.drawable.easy_trail_image),
    TrailImage(26, "Medium Trail", R.drawable.medium_trail_image),
    TrailImage(27, "Hard Trail", R.drawable.hard_trail_image),
    TrailImage(28, "Very Hard Trail", R.drawable.very_hard_trail_image),
    TrailImage(29, "Impossible Trail", R.drawable.impossible_trail_image),
    TrailImage(30, "Extreme Trail", R.drawable.extreme_trail_image)
)

@Composable
fun TrailCard(text: String, index: Int, modifier: Modifier = Modifier) {
    val trailImage = trailImageList[index]

    OutlinedCard(
        modifier = modifier
            .fillMaxWidth()
            .height(200.dp),
        shape = RoundedCornerShape(15.dp),
        border = BorderStroke(1.dp, Color.Black),
        colors = CardDefaults.cardColors(
            containerColor = MaterialTheme.colorScheme.surface,
        )
    ) {
        Box {
            Image(
                painter = painterResource(id = trailImage.imageResId),
                contentDescription = null,
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
                    text = text,
                    style = TextStyle(color = Color.Red, fontSize = 16.sp)
                )
            }
        }
    }
}
