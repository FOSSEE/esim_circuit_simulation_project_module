<?php
/**
 * Implementation of hook_mail().
 */
function circuit_simulation_mail($key, &$message, $params)
{
	global $user;
	$language = $message['language'];
	//$language = user_preferred_language($user);
	switch ($key)
	{
		case 'circuit_simulation_proposal_received':
			/* initializing data */
			$query = db_select('esim_circuit_simulation_proposal');
			$query->fields('esim_circuit_simulation_proposal');
			$query->condition('id', $params['circuit_simulation_proposal_received']['result1']);
			$query->range(0, 1);
			$proposal_data = $query->execute()->fetchObject();
			$user = user_load($params['circuit_simulation_proposal_received']['user_id']);
			if ($proposal_data->project_guide_name == "NULL" || $proposal_data->project_guide_name == "")
			{
				$project_guide_name = "Not Entered";
			} //$proposal_data->project_guide_name == NULL
			else
			{
				$project_guide_name = $proposal_data->project_guide_name;
			}
			if ($proposal_data->project_guide_email_id == "NULL" || $proposal_data->project_guide_email_id == "")
			{
				$project_guide_email_id = "Not Entered";
			} //$proposal_data->project_guide_email_id == NULL
			else
			{
				$project_guide_email_id = $proposal_data->project_guide_email_id;
			}

			$message['headers'] = $params['circuit_simulation_proposal_received']['headers'];
			$message['subject'] = t('[!site_name][Circuit Simulation Project] Your eSim Circuit Simulation Project proposal has been received', array(
				'!site_name' => variable_get('site_name', '')
			), array(
				'language' => $language->language
			));
			$message['body'] = array(
				'body' => t('
Dear !user_name,

We have received your eSim Circuit Simulation Project proposal with the following details:

Full Name: ' . $proposal_data->name_title . ' ' . $proposal_data->contributor_name . '
Email: ' . $user->mail . '
University/Institute: ' . $proposal_data->university . '
City: ' . $proposal_data->city . '
State: ' . $proposal_data->state . '
Country: ' . $proposal_data->country . '
Project Guide: ' . $project_guide_name . '
Project Guide Email: ' . $project_guide_email_id . '
Project Title: ' . $proposal_data->project_title . '

Your proposal is under review. You will soon receive an email when same has been approved/disapproved.

Best Wishes,

!site_name Team,
FOSSEE,IIT Bombay', array(
					'!site_name' => variable_get('site_name', ''),
					'!user_name' => $user->name
				), array(
					'language' => $language->language
				))
			);
			break;
		case 'circuit_simulation_proposal_disapproved':
			/* initializing data */
			$query = db_select('esim_circuit_simulation_proposal');
			$query->fields('esim_circuit_simulation_proposal');
			$query->condition('id', $params['circuit_simulation_proposal_disapproved']['proposal_id']);
			$query->range(0, 1);
			$proposal_data = $query->execute()->fetchObject();
			$user_data = user_load($params['circuit_simulation_proposal_disapproved']['user_id']);
			//$user = user_load($params['circuit_simulation_proposal']['name']);
			if ($proposal_data->project_guide_name == "NULL" || $proposal_data->project_guide_name == "")
			{
				$project_guide_name = "Not Entered";
			} //$proposal_data->project_guide_name == NULL
			else
			{
				$project_guide_name = $proposal_data->project_guide_name;
			}
			if ($proposal_data->project_guide_email_id == "NULL" || $proposal_data->project_guide_email_id == "")
			{
				$project_guide_email_id = "Not Entered";
			} //$proposal_data->project_guide_email_id == NULL
			else
			{
				$project_guide_email_id = $proposal_data->project_guide_email_id;
			}
			$message['headers'] = $params['circuit_simulation_proposal_disapproved']['headers'];
			$message['subject'] = t('[!site_name][Circuit Simulation Project] Your eSim Circuit Simulation Project proposal has been disapproved', array(
				'!site_name' => variable_get('site_name', '')
			), array(
				'language' => $language->language
			));
			$message['body'] = array(
				'body' => t('
Dear !user_name,

We regret to inform you that your eSim Circuit Simulation Project proposal with following details have been disapproved:

Full Name: ' . $proposal_data->name_title . ' ' . $proposal_data->contributor_name . '
Email: ' . $user_data->mail . '
University/Institute: ' . $proposal_data->university . '
City: ' . $proposal_data->city . '
State: ' . $proposal_data->state . '
Country: ' . $proposal_data->country . '
Project Guide: ' . $project_guide_name . '
Project Guide Email: ' . $project_guide_email_id . '
Project Title: ' . $proposal_data->project_title . '

Reason for rejection: ' . $proposal_data->dissapproval_reason . '


You are welcome to submit a new proposal.

Best Wishes,

!site_name Team,
FOSSEE,IIT Bombay', array(
					'!site_name' => variable_get('site_name', ''),
					'!user_name' => $user_data->name
				), array(
					'language' => $language->language
				))
			);
			break;
		case 'circuit_simulation_proposal_approved':
			/* initializing data */
			$url = 'http://esim.fossee.in/circuit-simulation-project/abstract-code';
			$query = db_select('esim_circuit_simulation_proposal');
			$query->fields('esim_circuit_simulation_proposal');
			$query->condition('id', $params['circuit_simulation_proposal_approved']['proposal_id']);
			$query->range(0, 1);
			$proposal_data = $query->execute()->fetchObject();
			$user_data = user_load($params['circuit_simulation_proposal_approved']['user_id']);

			if ($proposal_data->project_guide_name == "NULL" || $proposal_data->project_guide_name == "")
			{
				$project_guide_name = "Not Entered";
			} //$proposal_data->project_guide_name == NULL
			else
			{
				$project_guide_name = $proposal_data->project_guide_name;
			}
			if ($proposal_data->project_guide_email_id == "NULL" || $proposal_data->project_guide_email_id == "")
			{
				$project_guide_email_id = "Not Entered";
			} //$proposal_data->project_guide_email_id == NULL
			else
			{
				$project_guide_email_id = $proposal_data->project_guide_email_id;
			}
			$message['headers'] = $params['circuit_simulation_proposal_approved']['headers'];
			$message['subject'] = t('[!site_name][Circuit Simulation Project] Your eSim Circuit Simulation Project proposal has been approved', array(
				'!site_name' => variable_get('site_name', '')
			), array(
				'language' => $language->language
			));
			$message['body'] = array(
				'body' => t('
Dear !user_name,

Your eSim Circuit Simulation Project proposal with the following details has been approved:

Full Name: ' . $proposal_data->name_title . ' ' . $proposal_data->contributor_name . '
Email: ' . $user_data->mail . '
University/Institute: ' . $proposal_data->university . '
City: ' . $proposal_data->city . '
State: ' . $proposal_data->state . '
Country: ' . $proposal_data->country . '
Project Guide: ' . $project_guide_name . '
Project Guide Email: ' . $project_guide_email_id . '
Project Title: ' . $proposal_data->project_title . '

You can upload your abstract and the completed simulation at: ' . $url . '
Best Wishes,

!site_name Team,
FOSSEE,IIT Bombay', array(
					'!site_name' => variable_get('site_name', ''),
					'!user_name' => $user_data->name
				), array(
					'language' => $language->language
				))
			);
			break;
		case 'circuit_simulation_proposal_completed':
			/* initializing data */
			$query = db_select('esim_circuit_simulation_proposal');
			$query->fields('esim_circuit_simulation_proposal');
			$query->condition('id', $params['circuit_simulation_proposal_completed']['proposal_id']);
			$query->range(0, 1);
			$proposal_data = $query->execute()->fetchObject();
			$user_data = user_load($params['circuit_simulation_proposal_completed']['user_id']);
			if ($proposal_data->project_guide_name == "NULL" || $proposal_data->project_guide_name == "")
			{
				$project_guide_name = "Not Entered";
			} //$proposal_data->project_guide_name == NULL
			else
			{
				$project_guide_name = $proposal_data->project_guide_name;
			}
			if ($proposal_data->project_guide_email_id == "NULL" || $proposal_data->project_guide_email_id == "")
			{
				$project_guide_email_id = "Not Entered";
			} //$proposal_data->project_guide_email_id == NULL
			else
			{
				$project_guide_email_id = $proposal_data->project_guide_email_id;
			}
			$message['headers'] = $params['circuit_simulation_proposal_completed']['headers'];
			$message['subject'] = t('[!site_name][Circuit Simulation Project] Your eSim Circuit Simulation Project proposal has been completed', array(
				'!site_name' => variable_get('site_name', '')
			), array(
				'language' => $language->language
			));
			$message['body'] = array(
				'body' => t('
Dear !user_name,

Your eSim circuit simulation and abstract on the following process have been completed successfully.

Full Name: ' . $proposal_data->name_title . ' ' . $proposal_data->contributor_name . '
Email: ' . $user_data->mail . '
University/Institute: ' . $proposal_data->university . '
City: ' . $proposal_data->city . '
State: ' . $proposal_data->state . '
Country: ' . $proposal_data->country . '

Project Guide: ' . $project_guide_name . '
Project Guide Email: ' . $project_guide_email_id . '
Project Title: ' . $proposal_data->project_title . '
Best Wishes,

!site_name Team,
FOSSEE,IIT Bombay', array(
					'!site_name' => variable_get('site_name', ''),
					'!user_name' => $user_data->name
				), array(
					'language' => $language->language
				))
			);
			break;
		case 'circuit_simulation_proposal_deleted':
			/* initializing data */
			$query = db_select('esim_circuit_simulation_proposal');
			$query->fields('esim_circuit_simulation_proposal');
			$query->condition('id', $params['circuit_simulation_proposal_deleted']['proposal_id']);
			$query->range(0, 1);
			$proposal_data = $query->execute()->fetchObject();
			$user_data = user_load($params['circuit_simulation_proposal_deleted']['user_id']);
			
			if ($proposal_data->project_guide_name == "NULL" || $proposal_data->project_guide_name == "")
			{
				$project_guide_name = "Not Entered";
			} //$proposal_data->project_guide_name == NULL
			else
			{
				$project_guide_name = $proposal_data->project_guide_name;
			}
			if ($proposal_data->project_guide_email_id == "NULL" || $proposal_data->project_guide_email_id == "")
			{
				$project_guide_email_id = "Not Entered";
			} //$proposal_data->project_guide_email_id == NULL
			else
			{
				$project_guide_email_id = $proposal_data->project_guide_email_id;
			}
			$message['headers'] = $params['circuit_simulation_proposal_deleted']['headers'];
			$message['subject'] = t('[!site_name][Circuit Simulation Project] Your eSim Circuit Simulation Project proposal has been deleted', array(
				'!site_name' => variable_get('site_name', '')
			), array(
				'language' => $language->language
			));
			$message['body'] = array(
				'body' => t('
Dear !user_name,

Your eSim Circuit Simulation Project proposal with the following details has been deleted :

Full Name: ' . $proposal_data->name_title . ' ' . $proposal_data->contributor_name . '
Email: ' . $user_data->mail . '
University/Institute: ' . $proposal_data->university . '
City: ' . $proposal_data->city . '
State: ' . $proposal_data->state . '
Country: ' . $proposal_data->country . '
Project Guide: ' . $project_guide_name . '
Project Guide Email: ' . $project_guide_email_id . '
Project Title: ' . $proposal_data->project_title . '

You can propose a new circuit simulation project.

Best Wishes,

!site_name Team,
FOSSEE,IIT Bombay', array(
					'!site_name' => variable_get('site_name', ''),
					'!user_name' => $user_data->name
				), array(
					'language' => $language->language
				))
			);
			break;
		/**************************************************************/
		case 'abstract_uploaded':
			// $solution_q = db_query("SELECT * FROM {circuit_simulation_solution} WHERE id = %d LIMIT 1", $params['abstract_uploaded']['solution_id']);
			$query = db_select('esim_circuit_simulation_proposal');
			$query->fields('esim_circuit_simulation_proposal');
			$query->condition('id', $params['abstract_uploaded']['proposal_id']);
			$query->range(0, 1);
			$proposal_data = $query->execute()->fetchObject();
			
			$user = user_load($params['abstract_uploaded']['user_id']);
			$message['subject'] = t('[!site_name][Circuit Simulation Project] You have uploaded eSim circuit simulation project files', array(
				'!site_name' => variable_get('site_name', '')
			), array(
				'language' => $language->language
			));
			$message['headers'] = $params['abstract_uploaded']['headers'];
			$message['body'] = array(
				'body' => t('
Dear !user_name,

You have uploaded the following Abstract and Project files:

Project Title: ' . $proposal_data->project_title . '


The abstract and project files are under review. You will be notified when it has been approved.

Best Wishes,

!site_name Team,
FOSSEE,IIT Bombay', array(
					'!site_name' => variable_get('site_name', ''),
					'!user_name' => $user->name
				), array(
					'language' => $language->language
				))
			);
			break;
		case 'solution_approved':
			//$solution_q = db_query("SELECT * FROM {circuit_simulation_solution} WHERE id = %d LIMIT 1", $params['solution_approved']['solution_id']);
			$query = db_select('esim_circuit_simulation_solution');
			$query->fields('esim_circuit_simulation_solution');
			$query->condition('id', $params['solution_approved']['solution_id']);
			$query->range(0, 1);
			$solution_q = $query->execute();
			$solution_data = $solution_q->fetchObject();
			$query = db_select('esim_circuit_simulation_experiment');
			$query->fields('esim_circuit_simulation_experiment');
			$query->condition('id', $solution_data->experiment_id);
			$query->range(0, 1);
			$experiment_q = $query->execute();
			$experiment_data = $experiment_q->fetchObject();
			$user = user_load($params['solution_approved']['user_id']);
			$message['headers'] = $params['solution_approved']['headers'];
			$message['subject'] = t('[!site_name] Your uploaded eSim circuit simulation solution has been approved', array(
				'!site_name' => variable_get('site_name', '')
			), array(
				'language' => $language->language
			));
			$message['body'] = array(
				'body' => t('
Dear !user_name,

Your following solution has been approved:

Experiment Title: ' . $experiment_data->title . '

Please ensure that ALL the codes follow guidelines at http://esim.fossee.in/esim-circuit-simulation-project/esim-circuit-simulation-guidelines

Best Wishes,

!site_name Team,
FOSSEE,IIT Bombay', array(
					'!site_name' => variable_get('site_name', ''),
					'!user_name' => $user->name
				), array(
					'language' => $language->language
				))
			);
			break;
		case 'solution_disapproved':
			$user = user_load($params['solution_disapproved']['user_id']);
			$message['headers'] = $params['solution_disapproved']['headers'];
			$message['subject'] = t('[!site_name] Your uploaded eSim circuit-simulation solution has been disapproved', array(
				'!site_name' => variable_get('site_name', '')
			), array(
				'language' => $language->language
			));
			$message['body'] = array(
				'body' => t('
Dear !user_name,

Your following solution has been disapproved:

Solution number: ' . $params['solution_disapproved']['solution_number'] . '
Caption: ' . $params['solution_disapproved']['solution_caption'] . '

Reason for dis-approval: ' . $params['solution_disapproved']['message'] . '


Best Wishes,

!site_name Team,
FOSSEE,IIT Bombay', array(
					'!site_name' => variable_get('site_name', ''),
					'!user_name' => $user->name
				), array(
					'language' => $language->language
				))
			);
			break;
		case 'solution_deleted_user':
			$user = user_load($params['solution_deleted_user']['user_id']);
			$message['headers'] = $params['solution_deleted_user']['headers'];
			$message['subject'] = t('[!site_name] User has deleted pending eSim circuit-simulation solution', array(
				'!site_name' => variable_get('site_name', '')
			), array(
				'language' => $language->language
			));
			$message['body'] = array(
				'body' => t('
Dear !user_name,

Your following pending solution has been deleted :

Title of the Lab: ' . $params['solution_deleted_user']['lab_title'] . '
Title of the Experiment: ' . $params['solution_deleted_user']['experiment_title'] . '
Solution number: ' . $params['solution_deleted_user']['solution_number'] . '
Caption: ' . $params['solution_deleted_user']['solution_caption'] . '


Best Wishes,

!site_name Team,
FOSSEE,IIT Bombay', array(
					'!site_name' => variable_get('site_name', ''),
					'!user_name' => $user->name
				), array(
					'language' => $language->language
				))
			);
			break;
		case 'dependency_uploaded':
			$user = user_load($params['dependency_uploaded']['user_id']);
			$dependency_files = implode(',', $params['dependency_uploaded']['dependency_names']);
			$message['headers'] = $params['dependency_uploaded']['headers'];
			$message['subject'] = t('[!site_name] You have uploaded dependency file', array(
				'!site_name' => variable_get('site_name', '')
			), array(
				'language' => $language->language
			));
			$message['body'] = array(
				'body' => t('
Dear !user_name,

You have uploaded following dependency files : ' . $dependency_files . '

Please ensure that ALL the codes follow guidelines at http://esim.fossee.in/esim-circuit-simulation-project/esim-circuit-simulation-guidelines

Best Wishes,

!site_name Team,
FOSSEE,IIT Bombay', array(
					'!site_name' => variable_get('site_name', ''),
					'!user_name' => $user->name
				), array(
					'language' => $language->language
				))
			);
			break;
		case 'standard':
			$message['subject'] = $params['standard']['subject'];
			$message['body'] = $params['standard']['body'];
			$message['headers'] = $params['standard']['headers'];
			break;
	} //$key
}