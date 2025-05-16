import androidx.compose.foundation.background
import androidx.compose.foundation.layout.Column
import androidx.compose.foundation.layout.fillMaxSize
import androidx.compose.foundation.layout.padding
import androidx.compose.foundation.lazy.LazyColumn
import androidx.compose.material3.Button
import androidx.compose.material3.ButtonDefaults
import androidx.compose.material3.Text
import androidx.compose.runtime.*
import androidx.compose.ui.Modifier
import androidx.compose.ui.graphics.Color
import androidx.compose.ui.text.style.TextAlign
import androidx.compose.ui.unit.dp
import androidx.compose.ui.unit.sp
import com.example.HikingTrails.AppDatabase

@Composable
fun PageMain(database: AppDatabase) {
    // Get the initial value of timeModifier from the database
    var timeModifier by remember {
        mutableStateOf(database.GlobalVariablesDAO().getTimeModifier())
    }

    // Update the database when timeModifier changes
    LaunchedEffect(timeModifier) {
        database.GlobalVariablesDAO().updateTimeModifier(timeModifier)
    }

    LazyColumn(
        modifier = Modifier
            .fillMaxSize()
            .background(Color.Black) // Set the background color to black
    ) {
        item {
            Text(
                "About the app",
                modifier = Modifier.padding(10.dp),
                fontSize = 30.sp,
                color = Color.Green,
                textAlign = TextAlign.Center
            )
        }
        item {
            Text(
                "Hiking Trails is your ultimate companion " +
                        "for tracking your mountain adventures, categorized by difficulty levels. " +
                        "Monitor your progress, track time spent on various trails, and even adjust your walking pace. " +
                        "Embark on journeys, explore new horizons, " +
                        "and unlock achievements as you traverse the world with ease!",
                modifier = Modifier
                    .padding(horizontal = 20.dp)
                    .padding(bottom = 30.dp),
                color = Color.Green,
                fontSize = 19.sp,
                textAlign = TextAlign.Justify
            )
        }
        item {
            Text(
                "Please choose your level of experience so that the app can adjust trail times accordingly.",
                modifier = Modifier
                    .padding(horizontal = 20.dp)
                    .padding(bottom = 30.dp),
                color = Color.Green,
                fontSize = 19.sp,
                textAlign = TextAlign.Justify
            )
        }
        item {
            Column(modifier = Modifier.padding(horizontal = 20.dp)) {
                Button(
                    onClick = {
                        timeModifier = 2.0
                    },
                    colors = ButtonDefaults.buttonColors(
                        containerColor = if (timeModifier == 2.0) Color.Green else Color.Red,
                        contentColor = Color.Black // Adjust content color as needed
                    )
                ) {
                    Text("       I'm a slow walker       ")
                }
                Button(
                    onClick = {
                        timeModifier = 1.0
                    },
                    colors = ButtonDefaults.buttonColors(
                        containerColor = if (timeModifier == 1.0) Color.Green else Color.Red,
                        contentColor = Color.Black // Adjust content color as needed
                    )
                ) {
                    Text("I'm an experienced hiker")
                }
                Button(
                    onClick = {
                        timeModifier = 0.5
                    },
                    colors = ButtonDefaults.buttonColors(
                        containerColor = if (timeModifier == 0.5) Color.Green else Color.Red,
                        contentColor = Color.Black // Adjust content color as needed
                    )
                ) {
                    Text(" I'm a professional hiker ")
                }
            }
        }
    }
}
