using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class SetPlayerStartPoint : MonoBehaviour
{
    void Awake()
    {
        PlayerControl.instance.transform.position = transform.position;
        PlayerControl.instance.transform.rotation = transform.rotation;
        PlayerControl.instance.SetDestination(transform.position);
    }
    void Start()
    {
        PlayerControl.instance.transform.position = transform.position;
        PlayerControl.instance.transform.rotation = transform.rotation;
        PlayerControl.instance.SetDestination(transform.position);
    }
}
