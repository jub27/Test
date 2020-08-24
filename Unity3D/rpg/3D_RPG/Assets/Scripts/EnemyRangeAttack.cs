using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class EnemyRangeAttack : MonoBehaviour
{
    public Transform rangeWeaponPosition;
    public GameObject rangeWeapon;
    // Start is called before the first frame update
    void Start()
    {

    }

    // Update is called once per frame
    void Update()
    {
        
    }

    void ThrowWeapon()
    {
        Instantiate(rangeWeapon, rangeWeaponPosition.position, rangeWeaponPosition.rotation);
    }
}
