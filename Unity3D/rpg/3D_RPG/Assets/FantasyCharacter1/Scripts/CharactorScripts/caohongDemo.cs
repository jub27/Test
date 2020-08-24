using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class caorenDemo : MonoBehaviour {

    public GameObject attackBullet;
    public GameObject magicBullet;
    public GameObject magic2Bullet;
    public GameObject ultimateBullet;
    public GameObject damageEffect1;
    public GameObject damageEffect2;
    public GameObject damageEffect3;
    public GameObject damageEffect4;
    public GameObject damageEffect5;
    // Use this for initialization
    void Start () {
		
	}
	
	// Update is called once per frame
	void Update () {
		
	}
    IEnumerator delayBullet1()
    {
        int count = 2;
        for (int i = 0; i < count; i++)
        {
            GameObject obj = GameObject.Instantiate(damageEffect5);

            AttackedController c = GameObject.Find("bigzhangjiao (1)").GetComponent<AttackedController>();
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
            GameObject obj = GameObject.Instantiate(damageEffect5);

            AttackedController c = GameObject.Find("bigzhangjiao (1)").GetComponent<AttackedController>();
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
        int count = 6;
        for (int i = 0; i < count; i++)
        {
            GameObject obj = GameObject.Instantiate(damageEffect5);

            AttackedController c = GameObject.Find("bigzhangjiao (1)").GetComponent<AttackedController>();
            obj.transform.position = c.transform.position + new Vector3(Random.Range(-padding, padding), 0.12f, Random.Range(-padding, padding));
            yield return new WaitForSeconds(0.05f);
            if (i % 2 == 0)
            {
                c.attacked();

            }
        }
    }
    float padding = 1f;
    IEnumerator delayBullet()
    {
        int count = 1;
        for (int i = 0; i < count; i++)
        {
            GameObject obj = GameObject.Instantiate(damageEffect5);

            AttackedController c = GameObject.Find("bigzhangjiao (1)").GetComponent<AttackedController>();
            obj.transform.position = c.transform.position + new Vector3(Random.Range(-padding, padding), 0.12f, Random.Range(-padding, padding));
            yield return new WaitForSeconds(0.5f);
            if (i % 2 == 0)
            {
                c.attacked();

            }
        }


    }
    void preAction(string actionName)
    {
        AttackedController c = GameObject.Find("bigzhangjiao (1)").GetComponent<AttackedController>();
        string[] arr = actionName.Split('|');
        string name = arr[0];
        switch(name)
        {
            case AnimationName1.Attack:
                if (damageEffect1 != null)
                {
                    GameObject obj = GameObject.Instantiate(damageEffect1);
                    ParticlesEffect1 effect = obj.AddComponent<ParticlesEffect1>();
                    Transform target = GameObject.Find("bigzhangjiao (1)").transform;
                    effect.transform.position = target.position;
                    effect.play();
                }
                c.attacked();
                StartCoroutine(delayBullet());
                break;
            case AnimationName1.Magic:
                if (damageEffect2 != null)
                {
                    GameObject obj = GameObject.Instantiate(damageEffect2);
                    ParticlesEffect1 effect = obj.AddComponent<ParticlesEffect1>();
                    Transform target = GameObject.Find("bigzhangjiao (1)").transform;
                    effect.transform.position = MathUtil1.findChild(target, "attackedPivot").position;
                    effect.play();
                }
                c.attacked();
                StartCoroutine(delayBullet1());
                break;
            case AnimationName1.Magic2:
                if (damageEffect3 != null)
                {
                    GameObject obj = GameObject.Instantiate(damageEffect3);
                    ParticlesEffect1 effect = obj.AddComponent<ParticlesEffect1>();
                    Transform target = GameObject.Find("bigzhangjiao (1)").transform;
                    effect.transform.position = MathUtil1.findChild(target, "attackedPivot").position;
                    effect.play();
                }
                c.attacked();
                StartCoroutine(delayBullet2());
                break;
            case AnimationName1.Ultimate:

                if(damageEffect4 != null)
                {
                    GameObject obj = GameObject.Instantiate(damageEffect4);
                    ParticlesEffect1 effect = obj.AddComponent<ParticlesEffect1>();

                    effect.transform.position = GameObject.Find("bigzhangjiao (1)").transform.position;
                    effect.transform.rotation = Quaternion.Euler(0f, -111f, 0f);
                    effect.play();
                }
                c.attacked();
                StartCoroutine(delayBullet3());
                break;
        }
    }

    IEnumerator delayAttacked()
    {
        yield return new WaitForSeconds(1.5f);
        AttackedController c = GameObject.Find("bigzhangjiao (1)").GetComponent<AttackedController>();
        c.attacked();
        //yield return new WaitForSeconds(2.5f);
        //c.attacked();
    }
}
