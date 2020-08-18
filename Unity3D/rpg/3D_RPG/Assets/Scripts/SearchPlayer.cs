using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class SearchPlayer : MonoBehaviour
{
    EnemyControl ec;
    // Start is called before the first frame update
    void Start()
    {
        ec = transform.root.gameObject.GetComponent<EnemyControl>();
    }

    // Update is called once per frame
    void Update()
    {
        
    }

    private void OnTriggerEnter(Collider other)
    {
        if(ec.preAttack)
            transform.root.gameObject.GetComponent<EnemyControl>().SetChasing(other.transform.root.transform);
    }
}
