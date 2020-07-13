using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class Test : MonoBehaviour
{
    private void Start()
    {
        VolumeInfo info = new VolumeInfo();

        info.bytes = 10000;
        Debug.Log(info.kiloBytes);
        Debug.Log(info.megaBytes);

        info.megaBytes = 4;
        Debug.Log(info.bytes);
    }
}
