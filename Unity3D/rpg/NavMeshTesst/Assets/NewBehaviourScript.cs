using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.AI;
public class NewBehaviourScript : MonoBehaviour
{
    NavMeshAgent a;
    // Start is called before the first frame update
    void Start()
    {
        a = GetComponent<NavMeshAgent>();
        a.destination = GameObject.Find("dest").transform.position;
    }

    // Update is called once per frame
    void Update()
    {
        
    }
}
