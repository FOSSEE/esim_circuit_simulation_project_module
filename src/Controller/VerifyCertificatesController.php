<?php

namespace Drupal\circuit_simulation\Controller;

use Drupal\Core\Controller\ControllerBase;
use Drupal\Core\Database\Database;
use Symfony\Component\HttpFoundation\Request;
use Symfony\Component\DependencyInjection\ContainerInterface;
use Drupal\Core\Form\FormBuilderInterface;

class VerifyCertificatesController extends ControllerBase {

  protected $formBuilder;

  public function __construct(FormBuilderInterface $form_builder) {
    $this->formBuilder = $form_builder;
  }

  public static function create(ContainerInterface $container) {
    return new static(
      $container->get('form_builder')
    );
  }

  public function verifyCertificates($qr_code = NULL) {
  if (!empty($qr_code)) {
    return [
      '#markup' => $this->verifyQRCodeFromDB($qr_code),
    ];
  }
  else {
    $form = $this->formBuilder->getForm('Drupal\circuit_simulation\Form\VerifyCertificatesForm');
    return [
      '#type' => 'container',
      'form' => $form,
    ];
  }
}

  public function verifyQRCodeFromDB($qr_code) {
    $connection = Database::getConnection();
    $query = $connection->select('esim_circuit_simulation_qr_code', 'oqc')
      ->fields('oqc', ['proposal_id'])
      ->condition('qr_code', $qr_code);
    $result = $query->execute()->fetchObject();

    if ($result && $result->proposal_id) {
      $proposal_id = $result->proposal_id;

      $query = $connection->select('esim_circuit_simulation_proposal', 'ofp')
        ->fields('ofp')
        ->condition('approval_status', 3)
        ->condition('id', $proposal_id);
      $data = $query->execute()->fetchObject();

      if ($data) {
        $page_content = '<h4>Participation Details</h4>';
        $page_content .= '<table>';
        $page_content .= '<tr><td>Contributor Name</td><td>' . $data->contributor_name . '</td></tr>';
        $page_content .= '<tr><td>Project</td><td>OpenModelica Flowsheeting Project</td></tr>';
        $page_content .= '<tr><td>OpenModelica Flowsheeting completed</td><td>' . $data->project_title . '</td></tr>';
        if (!empty($data->project_guide_name)) {
          $page_content .= '<tr><td>Project Guide</td><td>' . $data->project_guide_name . '</td></tr>';
        }
        $page_content .= '</table>';

        return $page_content;
      }
    }

    return '<b>Sorry! The serial number you entered seems to be invalid. Please try again!</b>';
  }
}
