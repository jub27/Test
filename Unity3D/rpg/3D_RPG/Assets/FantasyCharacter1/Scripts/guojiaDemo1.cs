using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class guojiaDemo1 : MonoBehaviour {

    public GameObject attackBullet;
    public GameObject magicBullet;
    public GameObject magic2Bullet;
    public GameObject ultimateBullet;
    public GameObject damageEffect1;
    public GameObject damageEffect2;
    public GameObject damageEffect3;
    public GameObject damageEffect4;
	// Use this for initialization
	void Start () {
		
	}
	
	// Update is called once per frame
	void Update () {
		
	}

    void preAction(string actionName)
    {
        string[] arr = actionName.Split('|');
        string name = arr[0];
        switch(name)
        {
            case AnimationName1.Attack:
                if(attackBullet != null)
                {
                    GameObject obj = GameObject.Instantiate(attackBullet);
                    NormalBullet1 bullet = obj.GetComponent<NormalBullet1>();
                    bullet.player = transform;
                    bullet.target = GameObject.Find("bigzhangjiao (1)").transform;
                    bullet.effectObj = damageEffect1;
                    bullet.bulleting();
                }
                StartCoroutine(delayBullet());
                break;
            case AnimationName1.Magic:
                if (magicBullet != null)
                {
                    GameObject obj = GameObject.Instantiate(magicBullet);
                    NormalBullet1 bullet = obj.GetComponent<NormalBullet1>();
                    bullet.player = transform;
                    bullet.target = GameObject.Find("bigzhangjiao (1)").transform;
                    bullet.effectObj = damageEffect1;
                    bullet.bulleting();
                }
                StartCoroutine(delayBullet1());
                break;
            case AnimationName1.Magic2:
                if (magic2Bullet != null)
                {
                    GameObject obj = GameObject.Instantiate(magic2Bullet);
                    NormalBullet1 bullet = obj.GetComponent<NormalBullet1>();
                    bullet.player = transform;
                    bullet.target = GameObject.Find("bigzhangjiao (1)").transform;
                    bullet.effectObj = damageEffect2;
                    bullet.bulleting();
                }
                StartCoroutine(delayBullet2());
                break;
            case AnimationName1.Ultimate:
                StartCoroutine(delayBullet3());
                break;
        }
    }

    IEnumerator delayBullet1()
    {
        int count = 2;
        for (int i = 0; i < count; i++)
        {
            GameObject obj = GameObject.Instantiate(damageEffect4);

            AttackedController1 c = GameObject.Find("bigzhangjiao (1)").GetComponent<AttackedController1>();
            obj.transform.position = c.transform.position + new Vector3(Random.Range(-padding, padding), 0.12f, Random.Range(-padding, padding));
            yield return new WaitForSeconds(0.3f);
            if (i % 2 == 0)
            {
                c.attacked();

            }
        }
    }

    IEnumerator delayBullet2()
    {
        int count = 4;
        for (int i = 0; i < count; i++)
        {
            GameObject obj = GameObject.Instantiate(damageEffect4);

            AttackedController1 c = GameObject.Find("bigzhangjiao (1)").GetComponent<AttackedController1>();
            obj.transform.position = c.transform.position + new Vector3(Random.Range(-padding, padding), 0.12f, Random.Range(-padding, padding));
            yield return new WaitForSeconds(0.2f);
            if (i % 2 == 0)
            {
                c.attacked();

            }
        }
    }

    IEnumerator delayBullet3()
    {
        int count = 12;
        for (int i = 0; i < count; i++)
        {
            GameObject obj = GameObject.Instantiate(damageEffect4);

            AttackedController1 c = GameObject.Find("bigzhangjiao (1)").GetComponent<AttackedController1>();
            obj.transform.position = c.transform.position + new Vector3(Random.Range(-padding, padding), 0.12f, Random.Range(-padding, padding));
            yield return new WaitForSeconds(0.05f);
            if (i % 2 == 0)
            {
                c.attacked();

            }
        }
    }
    float padding = 3f;
    IEnumerator delayBullet()
    {
        int count = 1;
        for (int i = 0; i < count; i++)
        {
            GameObject obj = GameObject.Instantiate(damageEffect4);

            AttackedController1 c = GameObject.Find("bigzhangjiao (1)").GetComponent<AttackedController1>();
            obj.transform.position = c.transform.position + new Vector3(Random.Range(-padding, padding), 0.12f, Random.Range(-padding, padding));
            yield return new WaitForSeconds(0.5f);
            if (i % 2 == 0)
            {
                c.attacked();

            }
        }


    }
}
