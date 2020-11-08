using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class EnemySpawn : MonoBehaviour
{
    public int maxEnemyCount = 3;
    public GameObject enemyPrefab;
    private GameObject[] enemies;   
    public float spawnDelay = 7.0f;
    private float curTime;

    void Start()
    {
        enemies = new GameObject[maxEnemyCount]; // 몬스터 배열
        for(int i = 0; i < maxEnemyCount; i++)
        {
            enemies[i] = Instantiate(enemyPrefab, transform.position, transform.rotation);
            enemies[i].SetActive(false);
        }
        curTime = 0;
    }
    void Update()
    {
        curTime += Time.deltaTime;
        if(curTime >= spawnDelay) // spawnDelay 마다 몬스터 소환
        {
            SpawnEnemy();
            curTime = 0;
        }
    }
    void SpawnEnemy()
    {
        for(int i = 0; i < enemies.Length; i++)
        {
            if(!enemies[i].activeSelf)// 비활성화 되어있는 몬스터가 있다면 활성화
            {
                enemies[i].SetActive(true);
                return;
            }
        }
    }
}
