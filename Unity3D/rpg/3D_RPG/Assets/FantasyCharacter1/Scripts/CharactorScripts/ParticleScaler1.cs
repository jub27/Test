//This script will only work in editor mode. You cannot adjust the scale dynamically in-game!
using UnityEngine;
using System.Collections;

#if UNITY_EDITOR 
using UnityEditor;
#endif

public class ParticleScaler1 : MonoBehaviour 
{
	public float particleScale = 1.0f;
	public bool alsoScaleGameobject = true;
	float prevScale;

	void Start()
	{
		prevScale = particleScale;
	}

	void Update () 
	{
		if (prevScale != particleScale && particleScale > 0)
		{
			if (alsoScaleGameobject)
				transform.localScale = new Vector3(particleScale, particleScale, particleScale);

			float scaleFactor = particleScale / prevScale;


			//scale shuriken particle systems
			ScaleShurikenSystems(scaleFactor);

			//scale trail renders
			ScaleTrailRenderers(scaleFactor);

			prevScale = particleScale;
		}
	}

	void ScaleShurikenSystems(float scaleFactor)
	{
		//get all shuriken systems we need to do scaling on
		ParticleSystem[] systems = GetComponentsInChildren<ParticleSystem>();

		foreach (ParticleSystem system in systems)
		{
			system.startSpeed *= scaleFactor;
			system.startSize *= scaleFactor;
			system.gravityModifier *= scaleFactor;
		}
	}


	void ScaleTrailRenderers(float scaleFactor)
	{
		//get all animators we need to do scaling on
		TrailRenderer[] trails = GetComponentsInChildren<TrailRenderer>();

		//apply scaling to animators
		foreach (TrailRenderer trail in trails)
		{
			trail.startWidth *= scaleFactor;
			trail.endWidth *= scaleFactor;
		}
	}
}
