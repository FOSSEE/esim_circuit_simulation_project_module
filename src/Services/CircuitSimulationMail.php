<?php
namespace Drupal\circuit_simulation\CircuitSimulationMail;

use Drupal\Core\Mail\MailInterface;
use Drupal\Core\Mail\MailFormatHelper;
use Drupal\Core\StringTranslation\StringTranslationTrait;
use Drupal\Core\StringTranslation\TranslationInterface;

class CircuitSimulationMail {

  public function circuit_simulation_mail($key, &$message, $params) {
    switch ($key) {
      case 'circuit_simulation_proposal_received':
        $query = \Drupal::database()->select('esim_circuit_simulation_proposal', 'p')
          ->fields('p')
          ->condition('id', $params['circuit_simulation_proposal_received']['proposal_id'])
          ->range(0, 1);
        $proposal_data = $query->execute()->fetchObject();
        //var_dump($proposal_data);die;
        // Load user entity.

        $user = \Drupal\user\Entity\User::load($params['circuit_simulation_proposal_received']['user_id']);
        // Prepare the email message.
        $message['headers'] = $params['circuit_simulation_proposal_received']['headers'];
        $message['subject'] = t(
          '[@site_name][Circuit Simulation Project] Your eSim Circuit Simulation Project proposal has been received',
          ['@site_name' => \Drupal::config('system.site')->get('name')],
          ['langcode' => $language]
        );
        //var_dump($message);die;
        $message['body'][] = t(
          '
Dear @name,

We have received your eSim Circuit Simulation Project proposal with the following details:

Full Name: @full_name
Email: @user_email
University/Institute: @university
City: @city
State: @state
Country: @country
Project Title: @project_title

Your proposal is under review. You will soon receive an email when it has been approved/disapproved.

Best Wishes,

@site_name Team,
FOSSEE, IIT Bombay',
          [
            '@name' => $proposal_data->name_title . ' ' . $proposal_data->contributor_name,
            '@full_name' => $proposal_data->name_title . ' ' . $proposal_data->contributor_name,
            '@user_email' => $user->getEmail(),
            '@university' => $proposal_data->university,
            '@city' => $proposal_data->city,
            '@state' => $proposal_data->state,
            '@country' => $proposal_data->country,
            '@project_title' => $proposal_data->project_title,
            '@site_name' => \Drupal::config('system.site')->get('name'),
          ],
          ['langcode' => $language]
        );
        break;
    }
    //var_dump($message);die;
   // return $message;
  }
}