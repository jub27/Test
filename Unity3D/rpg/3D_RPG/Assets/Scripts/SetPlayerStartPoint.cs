using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class SetPlayerStartPoint : MonoBehaviour
{
    void Update()
    {
        if (PlayerControl.instance.startLoad == false)
        {
            PlayerControl.instance.transform.position = transform.position;
            PlayerControl.instance.SetDestination(transform.position);
        }
    }
}
