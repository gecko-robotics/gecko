# Notes

Misc notes and code snippets for things

```python
import numpy as np

# Create a dummy matrix
img = np.ones((50, 50, 3), dtype=np.uint8) * 255
# Save the shape of original matrix.
img_shape = img.shape

message_image = np.ndarray.tobytes(img)

re_img = np.frombuffer(message_image, dtype=np.uint8)

# Convert back the data to original image shape.
re_img = np.reshape(re_img, img_shape)
```

# log viewer

https://www.npmjs.com/package/frontail

    frontail /var/log/system.log /var/log/wifi.log

## namespace

Don't use it

```
Holly crap namespace and pickle use a lot of cpu!
zmq hs only 23%, but syncmanager is 77%
ns == msg image True
+------------------------------
| Alive processes: 11
+------------------------------
| subscribe[19339].............. cpu: 12.2%    mem: 0.10%
| subscribe[19343].............. cpu: 13.7%    mem: 0.10%
| SyncManager-1[19327].......... cpu: 77.4%    mem: 0.19%
| subscribe[19338].............. cpu: 12.5%    mem: 0.10%
| subscribe[19341].............. cpu: 13.6%    mem: 0.10%
| publish[19336]................ cpu: 8.7%    mem: 0.40%
| GeckoCore[19328].............. cpu: 23.3%    mem: 0.11%
| subscribe[19344].............. cpu: 13.8%    mem: 0.10%
| subscribe[19342].............. cpu: 13.7%    mem: 0.11%
| publish[19337]................ cpu: 8.8%    mem: 0.40%
| subscribe[19340].............. cpu: 13.7%    mem: 0.10%
```
