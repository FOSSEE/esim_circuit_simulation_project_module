<?php
namespace Drupal\circuit_simulation\Mail;

use Drupal\Core\Mail\MailInterface;
use Drupal\Core\Mail\MailFormatHelper;
use Drupal\Core\StringTranslation\StringTranslationTrait;
use Drupal\Core\StringTranslation\TranslationInterface;

class CircuitSimulationMail implements MailInterface {
  use StringTranslationTrait;

  public function __construct(TranslationInterface $string_translation) {
    $this->stringTranslation = $string_translation;
  }

  public function format(array $message) {
    $message['body'] = MailFormatHelper::wrapMail($message['body']);
    return $message;
  }

  public function mail(array $message) {
    switch ($message['key']) {
      case 'circuit_simulation_proposal_received':
        $query = $this->database->select('esim_circuit_simulation_proposal', 'p')
          ->fields('p')
          ->condition('id', $params['circuit_simulation_proposal_received']['proposal_id'])
          ->range(0, 1);
        $proposal_data = $query->execute()->fetchObject();

        // Load user entity.
        $user = $this->entityTypeManager->getStorage('user')->load($params['circuit_simulation_proposal_received']['user_id']);

        // Set default values for project guide details.
        $project_guide_name = $proposal_data->project_guide_name ?: $this->t('Not Entered');
        $project_guide_email_id = $proposal_data->project_guide_email_id ?: $this->t('Not Entered');

        // Prepare the email message.
        $message['headers'] = $params['circuit_simulation_proposal_received']['headers'];
        $message['subject'] = $this->t(
          '[!site_name][Circuit Simulation Project] Your eSim Circuit Simulation Project proposal has been received',
          ['!site_name' => \Drupal::config('system.site')->get('name')],
          ['langcode' => $language]
        );
        $message['body'] = $this->t(
          '
Dear @name,

We have received your eSim Circuit Simulation Project proposal with the following details:

Full Name: @full_name
Email: @user_email
University/Institute: @university
City: @city
State: @state
Country: @country
Project Guide: @project_guide_name
Project Guide Email: @project_guide_email
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
            '@project_guide_name' => $project_guide_name,
            '@project_guide_email' => $project_guide_email_id,
            '@project_title' => $proposal_data->project_title,
            '@site_name' => \Drupal::config('system.site')->get('name'),
          ],
          ['langcode' => $language]
        );
        break;
    }
    return $message;
  }
}
