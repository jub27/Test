using UnityEngine;
using System.Collections;
using System.Collections.Generic;
public class CurvelBullet1:Bullet1
{
	protected override void init ()
	{
		base.init ();
	
	}

    beisaier1 b;
    float time = 0f;
	public override void bulleting ()
	{
		base.bulleting ();

        Vector3 attackPos = MathUtil1.calcTargetPosByRotation(target, Random.Range(-60f, 60f), 15f);
        attackPos.y = 5f;
		transform.position = attackPos;
		stopParticle ();
        Vector3 attackedPos = MathUtil1.findChild(target, "attackedPivot").position;
        Vector3 pos1 = (attackedPos - transform.position) / 3 + transform.position;
        pos1.y += Random.Range(1f, 5f);
        Vector3 pos2 = 2f * (attackedPos - transform.position) / 3f + transform.position;
        pos2.y += Random.Range(0f, 1f);
        b = new beisaier1(transform.position, pos1, pos2, attackedPos);
		StartCoroutine (delayBulleting ());
	}

	IEnumerator delayBulleting()
	{
		yield return null;
		bulletState = BulletState1.line;
		startParticle ();
	}

	protected override void complete ()
	{
		base.complete ();
	}

	protected override void update ()
	{
		if(bulletState == BulletState1.none)
		{
			return;
		}
		base.update ();
		Vector3 attackedPos = MathUtil1.findChild(target, "attackedPivot").position;
        time += Time.deltaTime * speed;
        if (time >= 1f)
		{
			complete();
			return;
		}
        Vector3 pos = b.GetPointAtTime(time);
		transform.forward = pos - transform.position;
		transform.position = pos;
	}
}
