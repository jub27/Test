using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class EnemyRangeAttack : MonoBehaviour
{
    public Transform rangeWeaponPosition;
    public GameObject rangeWeapon;

    void ThrowWeapon()
    {
        Instantiate(rangeWeapon, rangeWeaponPosition.position, rangeWeaponPosition.rotation);
    }
}
