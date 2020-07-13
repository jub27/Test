using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class Spawner : MonoBehaviour
{
    public GameObject[] groups;

    private void Start()
    {
        spawnNext();
    }

    //groups에 포함된 Prefab들 중 하나를 랜덤하게 생성한다.
    public void spawnNext()
    {
        int i = Random.Range(0, groups.Length);

        Instantiate(groups[i], transform.position, Quaternion.identity);
    }
}
