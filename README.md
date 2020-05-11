<h1 align="center">DXBG 🌃</h1>
<p>
</p>

> Simple proof of concept that uses Direct3D to draw over the background.

### How does it work?

* Obtains Progman handle
* Sends WM_SPAWN to Progman
* Gets WorkerW handle
* Creates a new window
* Initializes Direct3D
* Shows window and set its parent to WorkerW's handle
  
### Sample usage

```cpp
int x = 1;
int y = 1;
int velocity_x = 16;
int velocity_y = 16;

void Draw()
{
	if (x <= 0 || x >= GetSystemMetrics(SM_CXSCREEN) - 100) velocity_x *= -1;
	if (y <= 0 || y >= GetSystemMetrics(SM_CYSCREEN) - 100) velocity_y *= -1;

	D3DRECT test_rect = { x, y, x + 100, y + 100 };
	direct_device->Clear(1, &test_rect, D3DCLEAR_TARGET | D3DCLEAR_TARGET, D3DCOLOR_ARGB(255, 255, 255, 255), 0, 0);

	x += velocity_x;
	y += velocity_y;
}
```

## Author

👤 **sonodima**

* Github: [@sonodima](https://github.com/sonodima)
