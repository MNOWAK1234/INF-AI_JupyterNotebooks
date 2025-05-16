import androidx.compose.foundation.layout.Spacer
import androidx.compose.foundation.layout.fillMaxSize
import androidx.compose.material.icons.Icons
import androidx.compose.material.icons.filled.Info
import androidx.compose.material3.Icon
import androidx.compose.material3.Text
import androidx.compose.runtime.Composable
import androidx.compose.ui.Modifier
import androidx.compose.ui.graphics.vector.ImageVector
import androidx.compose.foundation.layout.Column
import androidx.compose.foundation.layout.height
import androidx.compose.material.icons.filled.CheckCircle
import androidx.compose.material.icons.filled.Close
import androidx.compose.material.icons.filled.ThumbUp
import androidx.compose.material3.NavigationDrawerItem
import androidx.compose.ui.unit.dp

enum class MainRoute(value: String) {
    About("About"),
    EasyTrails("Easy trails"),
    HardTrails("Hard trails"),
    Rewards("Rewards")
}

data class DrawerMenu(val icon: ImageVector, val title: String, val option: String, val id: Int)

val menus = arrayOf(
    DrawerMenu(Icons.Filled.Info, "About", MainRoute.About.name, 0),
    DrawerMenu(Icons.Filled.ThumbUp, "Easy Trails", MainRoute.EasyTrails.name, 1),
    DrawerMenu(Icons.Filled.Close, "Hard Trails", MainRoute.HardTrails.name, 2),
    DrawerMenu(Icons.Filled.CheckCircle, "Achievements",MainRoute.Rewards.name, 3)
)

@Composable
fun DrawerContent(
    menus: Array<DrawerMenu>,
    onMenuClick: (DrawerMenu) -> Unit
) {
    Column(
        modifier = Modifier.fillMaxSize()
    ) {
        Spacer(modifier = Modifier.height(12.dp))
        menus.forEach {
            NavigationDrawerItem(
                label = { Text(text = it.title) },
                icon = { Icon(imageVector = it.icon, contentDescription = null) },
                selected = false,
                onClick = {
                    onMenuClick(it)
                }
            )
        }
    }
}